// javaconv_main.cpp -- command line for the sleelactl Java -> SLeeLa converter.
//
//   javaconv --jdk-version 28                 convert sleelactl/jdk/28/java -> sleelactl/jdk/28/sleela
//   javaconv --jdk DIR --out DIR [--jdk-version N]
//   javaconv --file Foo.java --out DIR
//   javaconv --self-check                     load and sanity-check the conversion model
//
// Exit codes: 0 every file converted, 1 some files failed closed, 2 usage/IO error.
#include "java_to_sleela.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

struct Cli {
    std::string jdk, out, file, version = "28", model, author = "unknown", root;
    bool selfCheck = false, quiet = false, json = false, bundle = false, split = false, singleThread = false, uncheckedSizes = false;
};

void usage(std::ostream& o) {
    o << "usage: javaconv --jdk-version N                     (uses sleelactl/jdk/N/java -> sleelactl/jdk/N/sleela)\n"
         "       javaconv --jdk DIR --out DIR [--jdk-version N]\n"
         "       javaconv --file File.java --out DIR\n"
         "       javaconv --self-check\n"
         "options: --model FILE  --author NAME  --quiet  --json  --bundle|--split  --single-thread  --unchecked-sizes\n"
         "         --single-thread: the program runs on one thread; synchronized and volatile become plain\n"
         "         --bundle: all types of one .java file in one runnable document (default for --file)\n"
         "         --split : one document per type, library layout (default for --jdk)\n"
         "exit: 0 all converted, 1 some files failed closed, 2 usage/IO error\n";
}

bool safeManifestPath(const std::string& raw) {
    if (raw.empty()) return false;
    const fs::path p(raw);
    if (p.is_absolute() || p.has_root_name() || p.has_root_directory()) return false;
    for (const auto& part : p) {
        const std::string name = part.string();
        if (name.empty() || name == "." || name == "..") return false;
    }
    return true;
}

bool pathIsWithin(const fs::path& root, const fs::path& candidate) {
    auto r = root.begin(), c = candidate.begin();
    for (; r != root.end(); ++r, ++c) if (c == candidate.end() || *r != *c) return false;
    return true;
}

bool removeOwnedOutput(const fs::path& root, const std::string& rel, std::string* why) {
    if (!safeManifestPath(rel)) {
        *why = "unsafe path in .javaconv-emitted: " + rel;
        return false;
    }
    std::error_code ec;
    const fs::path canonicalRoot = fs::weakly_canonical(root, ec);
    if (ec) {
        *why = "cannot resolve output root '" + root.string() + "': " + ec.message();
        return false;
    }
    const fs::path target = root / fs::path(rel);
    const fs::path canonicalParent = fs::weakly_canonical(target.parent_path(), ec);
    if (ec || !pathIsWithin(canonicalRoot, canonicalParent)) {
        *why = "previous output escapes output root through a symlink: " + target.string();
        return false;
    }
    const fs::file_status st = fs::symlink_status(target, ec);
    if (ec == std::errc::no_such_file_or_directory || st.type() == fs::file_type::not_found) return true;
    if (ec) {
        *why = "cannot inspect previous output '" + target.string() + "': " + ec.message();
        return false;
    }
    if (fs::is_symlink(st) || !fs::is_regular_file(st)) {
        *why = "refusing to remove non-regular previous output: " + target.string();
        return false;
    }
    if (!fs::remove(target, ec) || ec) {
        *why = "cannot remove previous output '" + target.string() + "': " + ec.message();
        return false;
    }
    return true;
}

bool readFile(const fs::path& p, std::string* out) {
    std::ifstream f(p, std::ios::binary);
    if (!f) return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    *out = ss.str();
    return true;
}

fs::path findRoot(const char* argv0, const std::string& hint) {
    if (!hint.empty()) return fs::path(hint);
    if (const char* e = std::getenv("SLEELA_ROOT")) return fs::path(e);
    std::error_code ec;
    fs::path exe = fs::weakly_canonical(fs::path(argv0), ec);
    // .../sleelactl/convert/build/javaconv  -> repo root is three levels above build/
    fs::path d = exe.parent_path();
    for (int i = 0; i < 6 && !d.empty(); i++, d = d.parent_path())
        if (fs::exists(d / "impl" / "subjects" / "conversion" / "conversion.model")) return d;
    return fs::current_path();
}

std::string jsonEscape(const std::string& s) {
    std::string o;
    for (unsigned char c : s) {
        switch (c) {
            case '"': o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\n"; break;
            case '\t': o += "\\t"; break;
            default:
                if (c < 0x20) { char b[8]; std::snprintf(b, sizeof b, "\\u%04x", c); o += b; }
                else o += (char)c;
        }
    }
    return o;
}

struct InputFile {
    fs::path path;
    fs::path rel;
    std::string source; // immutable snapshot shared by indexing and conversion
};

bool writeIfChanged(const fs::path& path, const std::string& bytes, std::string* why) {
    std::string old;
    if (readFile(path, &old) && old == bytes) return true;
    std::error_code ec;
    fs::create_directories(path.parent_path(), ec);
    if (ec) { *why = "cannot create output directory: " + ec.message(); return false; }
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) { *why = "cannot open output: " + path.string(); return false; }
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    out.close();
    if (!out) { *why = "cannot write output: " + path.string(); return false; }
    return true;
}

struct FileOutcome {
    std::string rel;
    bool ok = false;
    std::vector<javaconv::Diagnostic> diags;
    size_t emitted = 0;
};

}  // namespace

int main(int argc, char** argv) {
    Cli c;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        auto need = [&](std::string* dst) {
            if (i + 1 >= argc) { std::cerr << "javaconv: " << a << " needs a value\n"; usage(std::cerr); std::exit(2); }
            *dst = argv[++i];
        };
        if (a == "--jdk") need(&c.jdk);
        else if (a == "--out") need(&c.out);
        else if (a == "--file") need(&c.file);
        else if (a == "--jdk-version") need(&c.version);
        else if (a == "--model") need(&c.model);
        else if (a == "--author") need(&c.author);
        else if (a == "--root") need(&c.root);
        else if (a == "--self-check") c.selfCheck = true;
        else if (a == "--quiet") c.quiet = true;
        else if (a == "--json") c.json = true;
        else if (a == "--bundle") c.bundle = true;
        else if (a == "--single-thread") c.singleThread = true;
        else if (a == "--unchecked-sizes") c.uncheckedSizes = true;
        else if (a == "--split") c.split = true;
        else if (a == "-h" || a == "--help") { usage(std::cout); return 0; }
        else { std::cerr << "javaconv: unknown argument '" << a << "'\n"; usage(std::cerr); return 2; }
    }

    fs::path root = findRoot(argv[0], c.root);
    fs::path modelPath = c.model.empty() ? root / "impl" / "subjects" / "conversion" / "conversion.model" : fs::path(c.model);
    cm_model model;
    char err[512];
    if (cm_load(modelPath.string().c_str(), &model, err, sizeof err) != 0) {
        std::cerr << "javaconv: " << err << "\n";
        return 2;
    }

    if (c.selfCheck) {
        std::cout << "model " << model.name << " " << model.rev << " (" << modelPath.string() << ")\n"
                  << "  types=" << cm_count(&model, "type") << " binop=" << cm_count(&model, "binop")
                  << " unop=" << cm_count(&model, "unop") << " cast=" << cm_count(&model, "cast")
                  << " compound=" << cm_count(&model, "compound") << " ret=" << cm_count(&model, "ret")
                  << " unsupported=" << cm_count(&model, "unsupported") << "\n";
        // every int-width operator must have a template (the equivalence laws)
        static const char* const need[] = {"+", "-", "*", "/", "%", "<<", ">>", ">>>"};
        int missing = 0;
        for (const char* op : need)
            for (const char* cls : {"int", "long"})
                if (!cm_find(&model, "binop", cls, op, nullptr)) { std::cerr << "missing binop " << cls << " " << op << "\n"; missing++; }
        cm_free(&model);
        return missing ? 1 : 0;
    }

    std::vector<InputFile> inputs;  // sorted, immutable source snapshot
    fs::path outRoot;
    if (!c.file.empty()) {
        if (c.out.empty()) { std::cerr << "javaconv: --file needs --out\n"; return 2; }
        inputs.push_back({fs::path(c.file), fs::path(fs::path(c.file).filename()), {}});
        outRoot = c.out;
    } else {
        fs::path src;
        if (c.jdk.empty()) {
            src = root / "sleelactl" / "jdk" / c.version / "java";
            outRoot = c.out.empty() ? root / "sleelactl" / "jdk" / c.version / "sleela" : fs::path(c.out);
        } else {
            src = c.jdk;
            if (c.out.empty()) { std::cerr << "javaconv: --jdk needs --out\n"; return 2; }
            outRoot = c.out;
        }
        std::error_code ec;
        if (!fs::is_directory(src, ec)) {
            std::cerr << "javaconv: JDK source directory '" << src.string() << "' does not exist.\n"
                      << "  Place the presumed JDK " << c.version << " Java source tree there (see sleelactl/jdk/README.md).\n";
            return 2;
        }
        fs::recursive_directory_iterator it(src, ec);
        const fs::recursive_directory_iterator end;
        if (ec) {
            std::cerr << "javaconv: cannot walk source directory '" << src.string() << "': " << ec.message() << "\n";
            return 2;
        }
        while (it != end) {
            const fs::directory_entry entry = *it;
            std::error_code typeEc;
            const bool regular = entry.is_regular_file(typeEc);
            if (typeEc) {
                std::cerr << "javaconv: cannot inspect '" << entry.path().string() << "': " << typeEc.message() << "\n";
                return 2;
            }
            if (regular && entry.path().extension() == ".java") {
                const std::string fn = entry.path().filename().string();
                if (fn != "module-info.java" && fn != "package-info.java")
                    inputs.push_back({entry.path(), fs::relative(entry.path(), src), {}});
            }
            it.increment(ec);
            if (ec) {
                std::cerr << "javaconv: source walk failed under '" << src.string() << "': " << ec.message() << "\n";
                return 2;
            }
        }
        std::sort(inputs.begin(), inputs.end(), [](const InputFile& a, const InputFile& b) { return a.rel.generic_string() < b.rel.generic_string(); });
    }

    // Read each source exactly once. Both project indexing and translation consume
    // this immutable byte snapshot, so a changing source tree cannot produce a
    // mixed-generation symbol table and output.

    for (auto& in : inputs) {
        std::error_code sec;
        const fs::file_status st = fs::symlink_status(in.path, sec);
        if (sec || fs::is_symlink(st) || !fs::is_regular_file(st)) {
            std::cerr << "javaconv: refusing non-regular or symlink source: " << in.path.string() << "\n";
            cm_free(&model);
            return 2;
        }
        if (!readFile(in.path, &in.source)) {
            std::cerr << "javaconv: cannot read source: " << in.path.string() << "\n";
            cm_free(&model);
            return 2;
        }
    }

    // Pass 1 over the whole immutable snapshot: calls and field reads across files
    // are typed against one consistent source generation.
    javaconv::SymbolTable table;
    for (auto& in : inputs) {
        javaconv::Options o;
        o.sourceName = in.rel.generic_string();
        o.jdkVersion = c.version;
        o.singleThread = c.singleThread;
        o.checkedArrays = !c.uncheckedSizes;
        javaconv::collect(in.source, model, o, table);
    }

    // Keep the previous ownership manifest intact until the new output set is
    // known. Existing files may be replaced only when the prior manifest owns them.
    const fs::path emittedList = outRoot / ".javaconv-emitted";
    std::vector<std::string> previousOwned;
    {
        std::ifstream prev(emittedList);
        std::string rel;
        while (std::getline(prev, rel)) {
            if (rel.empty()) continue;
            if (!safeManifestPath(rel)) { std::cerr << "javaconv: unsafe path in .javaconv-emitted: " << rel << "\n"; return 2; }
            if (std::find(previousOwned.begin(), previousOwned.end(), rel) == previousOwned.end()) previousOwned.push_back(rel);
        }
        if (prev.bad()) { std::cerr << "javaconv: failed while reading " << emittedList.string() << "\n"; return 2; }
    }
    std::vector<std::string> emittedRel;

    std::vector<FileOutcome> outcomes;
    std::map<std::string, size_t> byConstruct;
    size_t emitted = 0;
    for (auto& in : inputs) {
        FileOutcome fo;
        fo.rel = in.rel.generic_string();
        javaconv::Options opt;
        opt.sourceName = fo.rel;
        opt.jdkVersion = c.version;
        opt.author = c.author;
        opt.singleThread = c.singleThread;
        opt.checkedArrays = !c.uncheckedSizes;
        opt.bundle = c.bundle || !c.file.empty();
        if (c.split) opt.bundle = false;
        opt.bundleName = in.path.stem().string();
        javaconv::Result r = javaconv::convert(in.source, model, opt, &table);
        if (r.ok() && !r.files.empty()) {
            fs::path dir = outRoot / in.rel.parent_path();
            std::error_code ec;
            fs::create_directories(dir, ec);
            if (ec) {
                std::cerr << "javaconv: cannot create output directory '" << dir.string() << "': " << ec.message() << "\n";
                return 2;
            }
            const fs::path canonicalRoot = fs::weakly_canonical(outRoot, ec);
            const fs::path canonicalDir = fs::weakly_canonical(dir, ec);
            if (ec || !pathIsWithin(canonicalRoot, canonicalDir)) {
                std::cerr << "javaconv: output directory escapes output root through a symlink: " << dir.string() << "\n";
                return 2;
            }
            for (auto& f : r.files) {
                const fs::path outputPath = dir / (f.typeName + ".sleela");
                const std::string relOutput = (in.rel.parent_path() / (f.typeName + ".sleela")).generic_string();
                if (!safeManifestPath(relOutput)) {
                    std::cerr << "javaconv: translator produced an unsafe output path: " << relOutput << "\n";
                    return 2;
                }
                if (std::find(emittedRel.begin(), emittedRel.end(), relOutput) != emittedRel.end()) {
                    std::cerr << "javaconv: two Java declarations map to the same output path: " << relOutput << "\n";
                    return 2;
                }
                std::error_code existsEc;
                const fs::file_status existing = fs::symlink_status(outputPath, existsEc);
                const bool exists = !existsEc && existing.type() != fs::file_type::not_found;
                const bool wasOwned = std::find(previousOwned.begin(), previousOwned.end(), relOutput) != previousOwned.end();
                if (exists && (fs::is_symlink(existing) || !fs::is_regular_file(existing))) {
                    std::cerr << "javaconv: refusing non-regular or symlink output: " << outputPath.string() << "\n";
                    return 2;
                }
                if (exists && !wasOwned) {
                    std::cerr << "javaconv: refusing to overwrite output not owned by the previous manifest: " << outputPath.string() << "\n";
                    return 2;
                }
                if (existsEc != std::errc::no_such_file_or_directory && existsEc) {
                    std::cerr << "javaconv: cannot inspect output path '" << outputPath.string() << "': " << existsEc.message() << "\n";
                    return 2;
                }
                std::string writeError;
                if (!writeIfChanged(outputPath, f.text, &writeError)) { std::cerr << "javaconv: " << writeError << "\n"; return 2; }
                emittedRel.push_back(relOutput);
                fo.emitted++;
                emitted++;
            }
            fo.ok = true;
        } else {
            fo.diags = r.diagnostics;
            if (fo.diags.empty()) {
                javaconv::Diagnostic d; d.construct = "syntax"; d.message = "no type declaration found";
                fo.diags.push_back(d);
            }
            for (auto& d : fo.diags) byConstruct[d.construct]++;
        }
        outcomes.push_back(fo);
    }

    // Remove only outputs owned by the previous run that are absent from this
    // run. Unchanged output bytes are never rewritten, preserving mtimes.
    for (const auto& oldRel : previousOwned) {
        if (std::find(emittedRel.begin(), emittedRel.end(), oldRel) == emittedRel.end()) {
            std::string why;
            if (!removeOwnedOutput(outRoot, oldRel, &why)) { std::cerr << "javaconv: " << why << "\n"; return 2; }
        }
    }
    std::sort(emittedRel.begin(), emittedRel.end());
    std::ostringstream manifest;
    for (const auto& rel : emittedRel) manifest << rel << "\n";
    std::string manifestError;
    if (!writeIfChanged(emittedList, manifest.str(), &manifestError)) { std::cerr << "javaconv: " << manifestError << "\n"; return 2; }
    size_t okFiles = 0, failed = 0;
    for (auto& o : outcomes) (o.ok ? okFiles : failed)++;

    // ---- report ----
    std::error_code ec;
    fs::create_directories(outRoot, ec);
    {
        std::ostringstream j;
        j << "{\n  \"model\": \"" << jsonEscape(model.name) << " " << jsonEscape(model.rev) << "\",\n"
          << "  \"jdkVersion\": \"" << jsonEscape(c.version) << "\",\n"
          << "  \"sourceFiles\": " << outcomes.size() << ",\n  \"converted\": " << okFiles << ",\n  \"failedClosed\": " << failed
          << ",\n  \"sleelaFilesEmitted\": " << emitted << ",\n  \"failuresByConstruct\": {";
        bool first = true;
        for (auto& kv : byConstruct) { j << (first ? "" : ", ") << "\"" << jsonEscape(kv.first) << "\": " << kv.second; first = false; }
        j << "},\n  \"failures\": [\n";
        first = true;
        for (auto& o : outcomes) {
            if (o.ok) continue;
            for (auto& d : o.diags) {
                j << (first ? "" : ",\n") << "    {\"file\": \"" << jsonEscape(o.rel) << "\", \"line\": " << d.line << ", \"col\": " << d.col
                  << ", \"construct\": \"" << jsonEscape(d.construct) << "\", \"message\": \"" << jsonEscape(d.message) << "\"}";
                first = false;
            }
        }
        j << "\n  ]\n}\n";
        std::string reportError;
        if (!writeIfChanged(outRoot / "CONVERSION-REPORT.json", j.str(), &reportError)) {
            std::cerr << "javaconv: " << reportError << "\n"; cm_free(&model); return 2;
        }
        if (c.json) std::cout << j.str();
    }
    {
        std::ostringstream m;
        m << "# Java -> SLeeLa conversion report\n\n"
          << "- model: `" << model.name << " " << model.rev << "`\n- presumed JDK version: " << c.version << "\n"
          << "- Java source files: " << outcomes.size() << "\n- converted: " << okFiles << " (" << emitted << " SLeeLa files)\n"
          << "- failed closed: " << failed << "\n\n"
          << "A file that fails closed produces NO output: a partly converted type is never written.\n\n";
        if (!byConstruct.empty()) {
            m << "## Failures by construct\n\n| construct | count |\n|---|---|\n";
            std::vector<std::pair<size_t, std::string>> v;
            for (auto& kv : byConstruct) v.push_back({kv.second, kv.first});
            std::sort(v.rbegin(), v.rend());
            for (auto& x : v) m << "| " << x.second << " | " << x.first << " |\n";
            m << "\n## Failed files\n\n";
            for (auto& o : outcomes) {
                if (o.ok) continue;
                m << "- `" << o.rel << "`: " << o.diags[0].line << ":" << o.diags[0].col << " [" << o.diags[0].construct << "] " << o.diags[0].message;
                if (o.diags.size() > 1) m << " (+" << (o.diags.size() - 1) << " more)";
                m << "\n";
            }
        }
        std::string reportError;
        if (!writeIfChanged(outRoot / "CONVERSION-REPORT.md", m.str(), &reportError)) {
            std::cerr << "javaconv: " << reportError << "\n"; cm_free(&model); return 2;
        }
    }

    if (!c.quiet) {
        std::cout << "javaconv: model " << model.name << " " << model.rev << ", JDK " << c.version << "\n"
                  << "  source files " << outcomes.size() << ", converted " << okFiles << " (" << emitted << " SLeeLa files), failed closed " << failed << "\n"
                  << "  output " << outRoot.string() << " (CONVERSION-REPORT.md / .json)\n";
        size_t shown = 0;
        for (auto& o : outcomes) {
            if (o.ok || shown >= 20) continue;
            std::cout << "  FAIL " << o.rel << ":" << o.diags[0].line << ":" << o.diags[0].col << " [" << o.diags[0].construct << "] " << o.diags[0].message << "\n";
            shown++;
        }
        if (failed > shown) std::cout << "  ... " << (failed - shown) << " more in the report\n";
    }
    cm_free(&model);
    return failed ? 1 : 0;
}
