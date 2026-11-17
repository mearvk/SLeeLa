// reg_model.cpp -- model-based / differential regression suite for the C core
//
// The C implementations (java_string, java_string_codec, utf16, regex, path)
// are driven through RAII wrappers and compared, step by step, against models
// built from the C++ standard library:
//
//   SLJavaString  <->  std::u16string          (random operation sequences)
//   UTF-8/UTF-16  <->  std::codecvt_utf8_utf16 (valid text only)
//   regex shapes  <->  std::regex
//   path normalize<->  std::regex_replace over std::string
//
// A disagreement at any step is a regression, whichever side is "right": the
// standard library is only a witness independent of the code under test.
#include "../../../impl/core/sleela_java_string.h"
#include "../../../impl/core/sleela_java_string_codec.h"
#include "../../../impl/core/sleela_path.h"
#include "../../../impl/core/sleela_regex.h"
#include "../../../impl/core/sleela_utf16.h"
#include "../c/regression.h"

#include <codecvt>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// RAII wrapper: owns an SLJavaString and releases it exactly once.
class JStr {
public:
    JStr() { sljava_string_init_empty(&s_); }
    explicit JStr(const std::u16string &u) {
        sljava_string_init_empty(&s_);
        (void)sljava_string_from_utf16(&s_, reinterpret_cast<const uint16_t *>(u.data()), u.size());
    }
    JStr(const JStr &) = delete;
    JStr &operator=(const JStr &) = delete;
    JStr(JStr &&o) noexcept : s_(o.s_) { sljava_string_init_empty(&o.s_); }
    ~JStr() { sljava_string_destroy(&s_); }
    SLJavaString *get() { return &s_; }
    const SLJavaString *get() const { return &s_; }
    std::u16string str() const {
        return std::u16string(reinterpret_cast<const char16_t *>(s_.units), s_.length);
    }
private:
    SLJavaString s_;
};

constexpr char16_t kAlphabet[] = {u'\0', u'a', u'b', u'Z', u'\x7F', u'\x80', u'\xE9', u'\x7FF', u'\x800',
                                  u'\xD7FF', u'\xD800', u'\xD83D', u'\xDBFF', u'\xDC00', u'\xDE00',
                                  u'\xDFFF', u'\xE000', u'\xFFFF'};

std::u16string random_u16(unsigned max_len) {
    std::u16string s;
    const unsigned n = reg_below(max_len + 1);
    for (unsigned i = 0; i < n; ++i) s.push_back(kAlphabet[reg_below(sizeof kAlphabet / sizeof kAlphabet[0])]);
    return s;
}

// Java String.hashCode over the model.
int32_t model_hash(const std::u16string &s) {
    uint32_t h = 0;
    for (char16_t c : s) h = 31u * h + c;
    int32_t out; std::memcpy(&out, &h, sizeof out);
    return out;
}

int sign(int32_t v) { return (v > 0) - (v < 0); }

bool is_hi(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
bool is_lo(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

int32_t model_code_point_at(const std::u16string &s, size_t i) {
    if (is_hi(s[i]) && i + 1 < s.size() && is_lo(s[i + 1]))
        return 0x10000 + ((static_cast<int32_t>(s[i]) - 0xD800) << 10) + (static_cast<int32_t>(s[i + 1]) - 0xDC00);
    return s[i];
}

bool all_match(const JStr (&regs)[3], const std::u16string (&model)[3]) {
    for (int i = 0; i < 3; ++i)
        if (regs[i].str() != model[i]) return false;
    return true;
}

// ---------------------------------------------------------------------------

void t_stateful_model_of_java_string() {
    reg_seed(0x57A7Eull);
    JStr regs[3];
    std::u16string model[3];
    long ops_done = 0;
    for (int step = 0; step < 40000; ++step) {
        const unsigned r = reg_below(3), x = reg_below(3), y = reg_below(3);
        switch (reg_below(11)) {
            case 0: {                                         // set
                const std::u16string v = random_u16(14);
                REG_EQ_I(0, sljava_string_from_utf16(regs[r].get(), reinterpret_cast<const uint16_t *>(v.data()), v.size()));
                model[r] = v; break;
            }
            case 1: {                                         // concat (destination may alias an operand)
                const std::u16string v = model[x] + model[y];
                REG_EQ_I(0, sljava_string_concat(regs[r].get(), regs[x].get(), regs[y].get()));
                model[r] = v; break;
            }
            case 2: {                                         // valid substring
                const std::u16string &m = model[x];
                const size_t b = reg_below(static_cast<uint32_t>(m.size() + 1));
                const size_t e = b + reg_below(static_cast<uint32_t>(m.size() - b + 1));
                const std::u16string v = m.substr(b, e - b);
                REG_EQ_I(0, sljava_string_substring(regs[r].get(), regs[x].get(), static_cast<int32_t>(b), static_cast<int32_t>(e)));
                model[r] = v; break;
            }
            case 3: {                                         // invalid substring: error, destination untouched
                const int32_t len = static_cast<int32_t>(model[x].size());
                int32_t b, e;
                switch (reg_below(4)) {
                    case 0: b = -1 - static_cast<int32_t>(reg_below(5)); e = 0; break;
                    case 1: b = 0; e = len + 1 + static_cast<int32_t>(reg_below(5)); break;
                    case 2: b = len; e = len - 1 - static_cast<int32_t>(reg_below(3)); break;
                    default: b = 2; e = 1; break;
                }
                REG_EQ_I(SL_JAVA_STRING_RANGE_OUT_OF_BOUNDS,
                         sljava_string_substring(regs[r].get(), regs[x].get(), b, e));
                break;
            }
            case 4: {                                         // charAt, in and out of range
                const int32_t i = static_cast<int32_t>(reg_below(static_cast<uint32_t>(model[x].size() + 3))) - 1;
                uint16_t c = 0;
                const int rc = sljava_string_char_at(regs[x].get(), i, &c);
                if (i >= 0 && static_cast<size_t>(i) < model[x].size()) {
                    REG_EQ_I(0, rc); REG_EQ_U(model[x][static_cast<size_t>(i)], c);
                } else {
                    REG_EQ_I(SL_JAVA_STRING_INDEX_OUT_OF_BOUNDS, rc);
                }
                break;
            }
            case 5: {                                         // codePointAt
                if (model[x].empty()) break;
                const size_t i = reg_below(static_cast<uint32_t>(model[x].size()));
                int32_t cp = -1;
                REG_EQ_I(0, sljava_string_code_point_at(regs[x].get(), static_cast<int32_t>(i), &cp));
                REG_EQ_I(model_code_point_at(model[x], i), cp);
                break;
            }
            case 6: {                                         // hashCode / equals / compareTo
                int32_t h = 0, ord = 0; int eq = -1;
                REG_EQ_I(0, sljava_string_hash_code(regs[x].get(), &h)); REG_EQ_I(model_hash(model[x]), h);
                REG_EQ_I(0, sljava_string_equals(regs[x].get(), regs[y].get(), &eq));
                REG_EQ_I(model[x] == model[y], eq);
                REG_EQ_I(0, sljava_string_compare_to(regs[x].get(), regs[y].get(), &ord));
                REG_EQ_I(sign(model[x].compare(model[y])), sign(ord));
                REG_CHECK((ord == 0) == (model[x] == model[y]));
                break;
            }
            case 7: {                                         // length
                int32_t n = -1;
                REG_EQ_I(0, sljava_string_length(regs[x].get(), &n));
                REG_EQ_U(model[x].size(), n); break;
            }
            case 8: {                                         // clone
                const std::u16string v = model[x];
                REG_EQ_I(0, sljava_string_clone(regs[r].get(), regs[x].get()));
                model[r] = v; break;
            }
            case 9: {                                         // codec round trip, then corrupt the record
                uint8_t *bytes = nullptr; size_t size = 0;
                REG_EQ_I(0, sljava_string_encode_record(regs[x].get(), &bytes, &size));
                REG_REQUIRE(bytes != nullptr);
                REG_EQ_U(12 + 2 * model[x].size(), size);
                JStr back;
                REG_EQ_I(0, sljava_string_decode_record(back.get(), bytes, size));
                REG_CHECK(back.str() == model[x]);
                bytes[reg_below(8)] ^= static_cast<uint8_t>(1u << reg_below(8));   // damage magic/version/reserved
                JStr bad(u"keep");
                REG_EQ_I(SL_JAVA_STRING_INVALID_ARGUMENT, sljava_string_decode_record(bad.get(), bytes, size));
                REG_CHECK(bad.str() == u"keep");
                std::free(bytes);
                break;
            }
            default: {                                        // code point counting & navigation
                const std::u16string &m = model[x];
                int32_t cnt = 0, idx = 0;
                size_t expect = 0;
                for (size_t i = 0; i < m.size(); ++i, ++expect) if (is_hi(m[i]) && i + 1 < m.size() && is_lo(m[i + 1])) ++i;
                REG_EQ_I(0, sljava_string_code_point_count(regs[x].get(), 0, static_cast<int32_t>(m.size()), &cnt));
                REG_EQ_U(expect, cnt);
                REG_EQ_I(0, sljava_string_offset_by_code_points(regs[x].get(), 0, cnt, &idx));
                REG_EQ_U(m.size(), idx);
                break;
            }
        }
        ++ops_done;
        if (!all_match(regs, model)) {
            reg_fail(__FILE__, __LINE__, "register/model divergence after step %d", step);
            return;
        }
    }
    REG_EQ_I(40000, ops_done);
}

// ---------------------------------------------------------------------------

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
void t_utf8_utf16_against_codecvt() {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    reg_seed(0xC0DEC5ull);
    for (int it = 0; it < 8000; ++it) {
        std::u16string u;
        const unsigned n = reg_below(30);
        for (unsigned i = 0; i < n; ++i) {
            uint32_t cp;
            switch (reg_below(5)) {
                case 0: cp = reg_below(0x80); break;
                case 1: cp = 0x80 + reg_below(0x780); break;
                case 2: cp = 0x800 + reg_below(0xD800 - 0x800); break;
                case 3: cp = 0xE000 + reg_below(0x10000 - 0xE000); break;
                default: cp = 0x10000 + reg_below(0x100000); break;
            }
            if (cp >= 0x10000) {
                cp -= 0x10000;
                u.push_back(static_cast<char16_t>(0xD800 + (cp >> 10)));
                u.push_back(static_cast<char16_t>(0xDC00 + (cp & 0x3FF)));
            } else {
                u.push_back(static_cast<char16_t>(cp));
            }
        }
        std::string utf8;
        try { utf8 = conv.to_bytes(u); } catch (const std::range_error &) { REG_CHECK(!"model rejected valid text"); return; }

        int64_t len = -1; uint16_t *units = nullptr; size_t count = 0;
        REG_EQ_I(0, sleela_utf8_utf16_length(utf8.data(), utf8.size(), &len));
        REG_EQ_U(u.size(), len);
        REG_EQ_I(0, sleela_utf8_to_utf16(utf8.data(), utf8.size(), &units, &count));
        REG_EQ_U(u.size(), count);
        if (units) REG_CHECK(std::u16string(reinterpret_cast<char16_t *>(units), count) == u);
        std::free(units);
        // and the reverse direction through the standard library
        if (!utf8.empty()) {
            uint16_t unit = 0;
            const size_t probe = reg_below(static_cast<uint32_t>(u.size()));
            REG_EQ_I(0, sleela_utf8_utf16_char_at(utf8.data(), utf8.size(), static_cast<int64_t>(probe), &unit));
            REG_EQ_U(u[probe], unit);
        }
    }
}
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

// ---------------------------------------------------------------------------

std::string rx_for(const std::string &cls, const std::string &quant) {
    std::string base = cls == "digit" ? "[0-9]" : cls == "word" ? "[A-Za-z0-9_]" : cls == "space" ? "[ \\t\\n\\v\\f\\r]" : "[\\s\\S]";
    return "^" + base + quant + "$";
}

void t_regex_shapes_against_std_regex() {
    const char *classes[] = {"digit", "word", "space", "any"};
    struct Q { const char *sleela; const char *ecma; };
    const Q quants[] = {{"", "{1}"}, {"+", "+"}, {"*", "*"}, {"{0}", "{0}"}, {"{1}", "{1}"}, {"{3}", "{3}"},
                        {"{1,2}", "{1,2}"}, {"{2,4}", "{2,4}"}, {"{0,3}", "{0,3}"}, {"{2,}", "{2,}"}, {"{0,}", "{0,}"}};
    const char alphabet[] = {'0', '7', 'a', 'Z', '_', ' ', '\t', '\n', '-', '.'};
    reg_seed(0x4E6E8ull);
    for (const char *cls : classes)
        for (const Q &q : quants) {
            const std::regex re(rx_for(cls, q.ecma), std::regex::ECMAScript);
            const std::string pattern = std::string("exact ") + cls + q.sleela;
            for (int it = 0; it < 400; ++it) {
                std::string text;
                const unsigned n = reg_below(7);
                for (unsigned i = 0; i < n; ++i) text.push_back(alphabet[reg_below(sizeof alphabet)]);
                SleelaRegexResult r;
                REG_EQ_I(SLEELA_REGEX_OK, sleela_regex_match(text.c_str(), pattern.c_str(), SLEELA_REGEX_LEVEL_SHAPE, &r));
                const bool want = std::regex_match(text, re);
                reg_note_check();
                if ((r.matched != 0) != want) {
                    reg_fail(__FILE__, __LINE__, "pattern \"%s\" text of %zu bytes: sleela=%d std::regex=%d",
                             pattern.c_str(), text.size(), r.matched, want ? 1 : 0);
                    return;
                }
                if (want) { REG_EQ_U(0, r.start); REG_EQ_U(text.size(), r.end); }
            }
        }
    // find family against std::string
    const char fa[] = {'a', 'b', ' ', '1'};
    for (int it = 0; it < 20000; ++it) {
        std::string t, lit;
        for (unsigned i = reg_below(9); i; --i) t.push_back(fa[reg_below(4)]);
        for (unsigned i = reg_below(4); i; --i) lit.push_back(fa[reg_below(4)]);
        const unsigned kind = reg_below(3);
        const char *prefix[] = {"contains ", "starts ", "ends "};
        SleelaRegexResult r;
        REG_EQ_I(0, sleela_regex_match(t.c_str(), (std::string(prefix[kind]) + lit).c_str(), SLEELA_REGEX_LEVEL_FIND, &r));
        size_t pos = std::string::npos;
        if (kind == 0) pos = t.find(lit);
        else if (kind == 1) pos = t.compare(0, lit.size(), lit) == 0 && lit.size() <= t.size() ? 0 : std::string::npos;
        else pos = lit.size() <= t.size() && t.compare(t.size() - lit.size(), lit.size(), lit) == 0 ? t.size() - lit.size() : std::string::npos;
        REG_EQ_I(pos != std::string::npos, r.matched);
        if (pos != std::string::npos) REG_EQ_U(pos, r.start);
    }
}

// ---------------------------------------------------------------------------

void t_path_normalize_against_regex_model() {
    const char alphabet[] = {'a', 'b', '.', '/', '\\', '/', ' '};
    const std::regex runs("/+");
    reg_seed(0x9A74ull);
    for (int it = 0; it < 20000; ++it) {
        std::string in;
        for (unsigned i = 1 + reg_below(16); i; --i) in.push_back(alphabet[reg_below(sizeof alphabet)]);
        std::string model = in;
        for (char &c : model) if (c == '\\') c = '/';
        model = std::regex_replace(model, runs, "/");
        if (model.size() > 1 && model.back() == '/') model.pop_back();
        char out[64];
        REG_EQ_I(0, slpath_normalize(out, sizeof out, in.c_str()));
        reg_note_check();
        if (model != out) { reg_fail(__FILE__, __LINE__, "normalize(\"%s\") = \"%s\", model \"%s\"", in.c_str(), out, model.c_str()); return; }
    }
}

}  // namespace

int main() {
    REG_RUN(t_stateful_model_of_java_string);
    REG_RUN(t_utf8_utf16_against_codecvt);
    REG_RUN(t_regex_shapes_against_std_regex);
    REG_RUN(t_path_normalize_against_regex_model);
    return reg_finish("model");
}
