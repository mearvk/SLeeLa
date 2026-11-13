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
    bool selfCheck = false, quiet = false, json = false, bundle = false, split = false, singleThread = false;
};

void usage(std::ostream& o) {
    o << "usage: javaconv --jdk-version N                     (uses sleelactl/jdk/N/java -> sleelactl/jdk/N/sleela)\n"
         "       javaconv --jdk DIR --out DIR [--jdk-version N]\n"
         "       javaconv --file File.java --out DIR\n"
         "       javaconv --self-check\n"
         "options: --model FILE  --author NAME  --quiet  --json  --bundle|--split  --single-thread\n"
         "         --single-thread: the program runs on one thread; synchronized and volatile become plain\n"
         "         --bundle: all types of one .java file in one runnable document (default for --file)\n"
         "         --split : one document per type, library layout (default for --jdk)\n"
         "exit: 0 all converted, 1 some files failed closed, 2 usage/IO error\n";
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
            case '\n': o += "\\n"; break;
            case '\t': o += "\\t"; break;
            default:
                if (c < 0x20) { char b[8]; std::snprintf(b, sizeof b, "\\u%04x", c); o += b; }
                else o += (char)c;
        }
    }
    return o;
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

    std::vector<std::pair<fs::path, fs::path>> inputs;  // source file, path relative to source root
    fs::path outRoot;
    if (!c.file.empty()) {
        if (c.out.empty()) { std::cerr << "javaconv: --file needs --out\n"; return 2; }
        inputs.push_back({fs::path(c.file), fs::path(fs::path(c.file).filename())});
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
        for (auto it = fs::recursive_directory_iterator(src, ec); it != fs::recursive_directory_iterator(); it.increment(ec)) {
            if (ec) break;
            if (!it->is_regular_file()) continue;
            fs::path p = it->path();
            if (p.extension() != ".java") continue;
            std::string fn = p.filename().string();
            if (fn == "module-info.java" || fn == "package-info.java") continue;
            inputs.push_back({p, fs::relative(p, src)});
        }
        std::sort(inputs.begin(), inputs.end());
    }

    // Pass 1 over the whole tree: record every type signature so that calls and field
    // reads across files are typed (the JDK is one program, not 900 separate ones).
    javaconv::SymbolTable table;
    for (auto& in : inputs) {
        std::string text;
        if (!readFile(in.first, &text)) continue;
        javaconv::Options o;
        o.sourceName = in.second.generic_string();
        o.jdkVersion = c.version;
        o.singleThread = c.singleThread;
        javaconv::collect(text, model, o, table);
    }

    std::vector<FileOutcome> outcomes;
    std::map<std::string, size_t> byConstruct;
    size_t emitted = 0;
    for (auto& in : inputs) {
        FileOutcome fo;
        fo.rel = in.second.generic_string();
        std::string text;
        if (!readFile(in.first, &text)) {
            javaconv::Diagnostic d; d.construct = "io"; d.message = "cannot read file";
            fo.diags.push_back(d);
            outcomes.push_back(fo);
            byConstruct["io"]++;
            continue;
        }
        javaconv::Options opt;
        opt.sourceName = fo.rel;
        opt.jdkVersion = c.version;
        opt.author = c.author;
        opt.singleThread = c.singleThread;
        opt.bundle = c.bundle || !c.file.empty();
        if (c.split) opt.bundle = false;
        opt.bundleName = in.first.stem().string();
        javaconv::Result r = javaconv::convert(text, model, opt, &table);
        if (r.ok() && !r.files.empty()) {
            fs::path dir = outRoot / in.second.parent_path();
            std::error_code ec;
            fs::create_directories(dir, ec);
            for (auto& f : r.files) {
                std::ofstream o(dir / (f.typeName + ".sleela"), std::ios::binary);
                o << f.text;
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
        std::ofstream("" + (outRoot / "CONVERSION-REPORT.json").string()) << j.str();
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
        std::ofstream("" + (outRoot / "CONVERSION-REPORT.md").string()) << m.str();
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
