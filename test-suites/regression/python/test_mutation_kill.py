"""Mutation testing of the C and C++ regression suites.

A regression suite that cannot fail proves nothing.  Each mutant below is a
one-line defect injected into a copy of the code under test (an off-by-one, a
dropped check, a wrong constant, a leak, a heap overrun).  The matching suite is
rebuilt against the mutant and MUST fail; a mutant that passes is a hole in the
suite (or an equivalent mutant, which must then be removed from this table).

The repository sources are never modified: mutants are compiled from copies
under test-suites/regression/build/mutants/.

Set REG_SKIP_MUTATION=1 to skip (about a minute of compiles).
"""
import os
import subprocess
import unittest
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

import regression_support as rs

CORE, FE = "impl/core/", "impl/frontend/"

# (id, logical library, source file, exact text to replace, replacement, suites that must catch it)
MUTANTS = [
    ("utf16-accept-surrogates", "utf16", CORE + "sleela_utf16.c", "(cp >= 0xd800 && cp <= 0xdfff) || cp > 0x10ffff", "cp > 0x10ffff", ["reg_utf16"]),
    ("utf16-accept-overlong-3byte", "utf16", CORE + "sleela_utf16.c", "(extra == 2 && cp < 0x800)", "(extra == 2 && cp < 0x80)", ["reg_utf16"]),
    ("utf16-accept-above-10ffff", "utf16", CORE + "sleela_utf16.c", "(cp >= 0xd800 && cp <= 0xdfff) || cp > 0x10ffff", "(cp >= 0xd800 && cp <= 0xdfff)", ["reg_utf16"]),
    ("utf16-accept-overlong-4byte", "utf16", CORE + "sleela_utf16.c", "(extra == 3 && cp < 0x10000)", "(extra == 3 && cp < 0x1000)", ["reg_utf16"]),
    ("utf16-wrong-low-surrogate-mask", "utf16", CORE + "sleela_utf16.c", "uint16_t low = (uint16_t)(0xdc00 + (value & 0x3ff));", "uint16_t low = (uint16_t)(0xdc00 + (value & 0x1ff));", ["reg_utf16"]),
    ("utf16-stale-output-on-error", "utf16", CORE + "sleela_utf16.c", "    *out_units = NULL;\n    *out_code_units = 0;\n", "    *out_code_units = 0;\n", ["reg_utf16"]),
    ("jstr-hash-multiplier", "java_string", CORE + "sleela_java_string.c", "hash = hash * 31u + (uint32_t)value->units[i];", "hash = hash * 33u + (uint32_t)value->units[i];", ["reg_java_string", "reg_model"]),
    ("jstr-compare-sign-flipped", "java_string", CORE + "sleela_java_string.c", "*out_order = (int32_t)left->length - (int32_t)right->length;", "*out_order = (int32_t)right->length - (int32_t)left->length;", ["reg_java_string", "reg_model"]),
    ("jstr-codepoint-pairs-any-low", "java_string", CORE + "sleela_java_string.c", "is_low_surrogate(value->units[index + 1])) {", "1) {", ["reg_java_string", "reg_model"]),
    ("jstr-substring-off-by-one", "java_string", CORE + "sleela_java_string.c", "memcpy(copy, value->units + begin_index, length * sizeof(*copy));", "memcpy(copy, value->units + begin_index + 1, length * sizeof(*copy));", ["reg_java_string", "reg_model"]),
    ("jstr-concat-leaks-destination", "java_string", CORE + "sleela_java_string.c", "    sljava_string_destroy(out);\n    out->units = joined;", "    out->units = joined;", ["reg_java_string"]),
    ("jstr-concat-heap-overrun", "java_string", CORE + "sleela_java_string.c", "right->length * sizeof(*joined));", "right->length * sizeof(*joined) + 2);", ["reg_java_string"]),
    ("jstr-charat-accepts-length", "java_string", CORE + "sleela_java_string.c", "(size_t)index >= value->length)\n        return SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS;\n    *out_unit", "(size_t)index > value->length)\n        return SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS;\n    *out_unit", ["reg_java_string", "reg_model"]),
    ("codec-ignores-version-byte", "java_string_codec", CORE + "sleela_java_string_codec.c", "bytes[4] != 1u || bytes[5] != 0u", "bytes[5] != 0u", ["reg_java_string_codec", "reg_model"]),
    ("codec-accepts-trailing-bytes", "java_string_codec", CORE + "sleela_java_string_codec.c", "size != SL_JAVA_STRING_RECORD_HEADER_SIZE + (size_t)length * 2u)", "size < SL_JAVA_STRING_RECORD_HEADER_SIZE + (size_t)length * 2u)", ["reg_java_string_codec"]),
    ("codec-length-byte-shift", "java_string_codec", CORE + "sleela_java_string_codec.c", "bytes[9] = (uint8_t)(value->length >> 8);", "bytes[9] = (uint8_t)(value->length >> 7);", ["reg_java_string_codec", "reg_model"]),
    ("codec-unit-high-byte-shift", "java_string_codec", CORE + "sleela_java_string_codec.c", "(uint8_t)(unit >> 8);", "(uint8_t)(unit >> 7);", ["reg_java_string_codec", "reg_model"]),
    ("regex-word-excludes-underscore", "regex", CORE + "sleela_regex.c", "ok = isalnum(c) || c == '_';", "ok = isalnum(c);", ["reg_regex", "reg_model"]),
    ("regex-ignores-minimum-count", "regex", CORE + "sleela_regex.c", "return count >= min_count && count == n;", "return count == n;", ["reg_regex", "reg_model"]),
    ("regex-exact-count-off-by-one", "regex", CORE + "sleela_regex.c", "if (name[j] == '}') { min = a; max = a; ++j; }", "if (name[j] == '}') { min = a; max = a + 1; ++j; }", ["reg_regex", "reg_model"]),
    ("regex-level3-supported", "regex", CORE + "sleela_regex.c", "if (level >= SLEELA_REGEX_LEVEL_STRUCTURE)", "if (level > SLEELA_REGEX_LEVEL_STRUCTURE)", ["reg_regex"]),
    ("regex-plus-means-star", "regex", CORE + "sleela_regex.c", "if (name[i] == '+') { min = 1;", "if (name[i] == '+') { min = 0;", ["reg_regex", "reg_model"]),
    ("regex-quantifier-overflow-unchecked", "regex", CORE + "sleela_regex.c", "if (a > (SIZE_MAX - d) / 10) return 0;", "if (0) return 0;", ["reg_regex"]),
    ("time-negative-seconds-truncate", "time", CORE + "sleela_time.c", "if(ms%1000<0)q-=1;", "", ["reg_time_memory"]),
    ("time-json-truncation-silent", "time", CORE + "sleela_time.c", "if((size_t)w>=n){out[0]=0;return ERANGE;}", "if(0){out[0]=0;return ERANGE;}", ["reg_time_memory"]),
    ("lexer-noteq-text-truncated", "lexer", FE + "lexer.cpp", 'emit(Tok::NotEq,"!=")', 'emit(Tok::NotEq,"!")', ["reg_lexer"]),
    ("events-wait-never-blocks", "events", CORE + "sleela_events.c", "while (!obj->state && rc == 0) rc = pthread_cond_timedwait(&obj->cond, &obj->lock, &deadline);", "", ["reg_events"]),
    ("events-set-wakes-one-waiter", "events", CORE + "sleela_events.c", "pthread_cond_broadcast(&obj->cond);", "pthread_cond_signal(&obj->cond);", ["reg_events"]),
    ("events-invalid-handle-dereferenced", "events", CORE + "sleela_events.c", "return (event == 0 || event == SL_EVENT_INVALID) ? NULL : (SLEventObject*)(intptr_t)event;", "return event == 0 ? NULL : (SLEventObject*)(intptr_t)event;", ["reg_events"]),
    ("events-signal-handler-one-shot", "events", CORE + "sleela_events.c", "action.sa_flags = SA_RESTART;", "action.sa_flags = SA_RESETHAND;", ["reg_events"]),
    ("events-queue-capacity-off-by-one", "events", CORE + "sleela_events.c", "if (sl_queue_count == SL_EVENT_QUEUE_CAPACITY) {", "if (sl_queue_count == SL_EVENT_QUEUE_CAPACITY - 1) {", ["reg_events"]),
    ("events-poll-does-not-advance", "events", CORE + "sleela_events.c", "sl_queue_head = (sl_queue_head + 1U) % SL_EVENT_QUEUE_CAPACITY;", "sl_queue_head = sl_queue_head % SL_EVENT_QUEUE_CAPACITY;", ["reg_events"]),
    ("path-root-collapses-to-empty", "path", CORE + "sleela_path.c", "while (j > 1 && out[j - 1] == SL_PATH_NATIVE_SEPARATOR) --j;", "while (j > 0 && out[j - 1] == SL_PATH_NATIVE_SEPARATOR) --j;", ["reg_path", "reg_model"]),
    ("path-normalize-off-by-one-write", "path", CORE + "sleela_path.c", "if (j + 1 >= out_size) return -1;", "if (j + 1 > out_size) return -1;", ["reg_path"]),
    ("path-mkdir-not-idempotent", "path", CORE + "sleela_path.c", "if (slpath_is_directory(path)) return 0;", "if (0) return 0;", ["reg_path"]),
    ("path-absolute-needs-existing-file", "path", CORE + "sleela_path.c", "    if (path[0] == '/') {\n        return slpath_normalize(out, out_size, path);\n    }", "    if (path[0] == '/') {\n        return -1;\n    }", ["reg_path"]),
    ("time-iso-buffer-contract", "time", CORE + "sleela_time.c", "n<32||!utc_tm(ms,&t))return EINVAL;", "n<24||!utc_tm(ms,&t))return EINVAL;", ["reg_time_memory"]),
    ("time-http-buffer-contract", "time", CORE + "sleela_time.c", "if(!out||n<30||!utc_tm(ms,&t))", "if(!out||n<20||!utc_tm(ms,&t))", ["reg_time_memory"]),
    ("time-month-zero-based", "time", CORE + "sleela_time.c", "t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour", "t.tm_year+1900,t.tm_mon,t.tm_mday,t.tm_hour", ["reg_time_memory"]),
    ("time-bodi-last-byte", "time", CORE + "sleela_time.c", "out[31]=b;", "out[31]=b+1;", ["reg_time_memory"]),
    ("time-sleep-returns-early", "time", CORE + "sleela_time.c", "req.tv_nsec=(long)((millis%1000)*1000000L);", "req.tv_nsec=(long)((millis%1000)*100000L);", ["reg_time_memory"]),
    ("time-json-size-contract", "time", CORE + "sleela_time.c", "if(!s||!out||n<64)return EINVAL;", "if(!s||!out||n<32)return EINVAL;", ["reg_time_memory"]),
    ("memory-calloc-not-zeroed", "memory", CORE + "sleela_memory.c", "return calloc(count, size);", "return malloc(count * size);", ["reg_time_memory"]),
    ("memory-alloc-one-byte-short", "memory", CORE + "sleela_memory.c", "    return malloc(size);", "    return malloc(size ? size - 1 : 0);", ["reg_time_memory"]),
    ("lexer-non-sealed-keyword-lost", "lexer", FE + "lexer.cpp", '{"non-sealed",Tok::KwNonSealed},', "", ["reg_lexer"]),
    ("lexer-hyphen-always-joins", "lexer", FE + "lexer.cpp", "(peek()=='-'&&identChar(peek(1)))", "peek()=='-'", ["reg_lexer"]),
    ("lexer-char-literal-astral-allowed", "lexer", FE + "lexer.cpp", "if(cp>0xFFFF)error(", "if(cp>0x10FFFF)error(", ["reg_lexer"]),
    ("lexer-ushr-assign-misnamed", "lexer", FE + "lexer.cpp", 'emit(Tok::UshrAssign,">>>=")', 'emit(Tok::ShrAssign,">>>=")', ["reg_lexer"]),
    ("lexer-shift-opens-comment-midline", "lexer", FE + "lexer.cpp", "else if(atLinePrefix()&&c=='<'&&peek(1)=='<')", "else if(c=='<'&&peek(1)=='<')", ["reg_lexer"]),
    ("lexer-nul-escape-wrong", "lexer", FE + "lexer.cpp", "case'0':out+='\\0';break;", "case'0':out+='0';break;", ["reg_lexer"]),
    ("lexer-triple-colon-lost", "lexer", FE + "lexer.cpp", 'emit(Tok::TripleColon,":::")', 'emit(Tok::DoubleColon,":::")', ["reg_lexer"]),
    ("lexer-column-zero-based-after-newline", "lexer", FE + "lexer.cpp", "if(c=='\\n'){line_++;col_=1;}", "if(c=='\\n'){line_++;col_=0;}", ["reg_lexer"]),
    ("lexer-unicode-escape-radix", "lexer", FE + "lexer.cpp", "cp=(cp<<4)|v;}out+=utf8FromCodePoint(cp)", "cp=(cp<<3)|v;}out+=utf8FromCodePoint(cp)", ["reg_lexer"]),
]


def build_mutant_object(lib: str, rel: str, old: str, new: str, ident: str) -> Path:
    src = rs.ROOT / rel
    text = src.read_text(encoding="utf-8")
    if text.count(old) != 1:
        raise AssertionError(f"{ident}: anchor text occurs {text.count(old)} times in {rel}; the table is stale")
    out_dir = rs.BUILD / "mutants" / ident
    out_dir.mkdir(parents=True, exist_ok=True)
    mutated = out_dir / src.name
    mutated.write_text(text.replace(old, new, 1), encoding="utf-8")
    cxx = src.suffix == ".cpp"
    return rs.compile_object(mutated, out_dir / (src.stem + ".o"), cxx=cxx,
                             flags=[*(rs.LIB_CXX if cxx else rs.LIB_C), f"-I{src.parent}"])


def kill(mutant):
    ident, lib, rel, old, new, suites = mutant
    obj = build_mutant_object(lib, rel, old, new, ident)
    verdicts = {}
    for suite in suites:
        if lib not in rs.SUITES[suite][1]:
            raise AssertionError(f"{ident}: {suite} does not link {lib}")
        exe = rs.build_suite(suite, replace={lib: obj}, tag=ident)
        try:
            res = rs.run_binary(exe, timeout=60)
            verdicts[suite] = res.returncode != 0
        except subprocess.TimeoutExpired:
            verdicts[suite] = True          # a hang (e.g. a lost wake-up) is a detected defect
    return ident, verdicts


@unittest.skipIf(os.environ.get("REG_SKIP_MUTATION") == "1", "REG_SKIP_MUTATION=1")
@unittest.skipIf(rs.have_toolchain(), rs.have_toolchain() or "")
class MutationKill(unittest.TestCase):
    def test_mutant_table_has_no_duplicates_and_covers_every_library(self):
        ids = [m[0] for m in MUTANTS]
        self.assertEqual(len(ids), len(set(ids)))
        self.assertEqual({m[1] for m in MUTANTS}, set(rs.SOURCES), "every code-under-test library needs mutants")

    def test_every_injected_defect_is_detected(self):
        for lib in rs.SOURCES:                       # baseline objects first (single-threaded, cached)
            rs.library_object(lib)
        rs.support_object()
        for suite in rs.SUITES:
            self.assertEqual(rs.run_binary(rs.build_suite(suite)).returncode, 0, f"baseline {suite} must pass")
        with ThreadPoolExecutor(max_workers=max(2, min(6, os.cpu_count() or 2))) as pool:
            results = list(pool.map(kill, MUTANTS))
        survivors = [(ident, [s for s, dead in v.items() if not dead]) for ident, v in results if not all(v.values())]
        self.assertEqual(survivors, [], "mutants that the regression suites failed to detect: " + repr(survivors))
        self.assertEqual(len(results), len(MUTANTS))


if __name__ == "__main__":
    unittest.main()
