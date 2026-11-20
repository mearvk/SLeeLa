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
#include <cstdint>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

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

std::string sha256Hex(const std::string& msg){
    auto rotr=[](uint32_t x,uint32_t n){return (x>>n)|(x<<(32-n));};
    static const uint32_t K[64]={
      0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
      0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
      0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
      0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
      0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
      0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
      0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
      0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
    uint32_t h[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    std::string data=msg; uint64_t bitlen=(uint64_t)data.size()*8;
    data.push_back((char)0x80);
    while(data.size()%64!=56) data.push_back((char)0x00);
    for(int i=7;i>=0;--i) data.push_back((char)((bitlen>>(i*8))&0xff));
    for(size_t off=0;off<data.size();off+=64){
        uint32_t w[64];
        for(int i=0;i<16;++i){ w[i]=((uint32_t)(uint8_t)data[off+i*4]<<24)|((uint32_t)(uint8_t)data[off+i*4+1]<<16)|((uint32_t)(uint8_t)data[off+i*4+2]<<8)|((uint32_t)(uint8_t)data[off+i*4+3]); }
        for(int i=16;i<64;++i){ uint32_t s0=rotr(w[i-15],7)^rotr(w[i-15],18)^(w[i-15]>>3); uint32_t s1=rotr(w[i-2],17)^rotr(w[i-2],19)^(w[i-2]>>10); w[i]=w[i-16]+s0+w[i-7]+s1; }
        uint32_t a=h[0],b=h[1],c=h[2],d=h[3],e=h[4],f=h[5],g=h[6],hh=h[7];
        for(int i=0;i<64;++i){ uint32_t S1=rotr(e,6)^rotr(e,11)^rotr(e,25); uint32_t ch=(e&f)^((~e)&g); uint32_t t1=hh+S1+ch+K[i]+w[i]; uint32_t S0=rotr(a,2)^rotr(a,13)^rotr(a,22); uint32_t maj=(a&b)^(a&c)^(b&c); uint32_t t2=S0+maj; hh=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2; }
        h[0]+=a;h[1]+=b;h[2]+=c;h[3]+=d;h[4]+=e;h[5]+=f;h[6]+=g;h[7]+=hh;
    }
    static const char* hex="0123456789abcdef"; std::string out; out.reserve(64);
    for(int i=0;i<8;++i) for(int j=7;j>=0;--j) out.push_back(hex[(h[i]>>(j*4))&0xf]);
    return out;
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
            case '\n': o += "\\n"; break;
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

/*
 * Every conversion is assembled in a sibling staging directory. The old output
 * tree is copied without following symlinks; all generated files, ownership
 * metadata, provenance and reports are written to the stage. Only after every
 * write succeeds is the directory exchanged with the previous generation.
 * Keeping stage and destination under the same parent keeps renames on one
 * filesystem. Any pre-publication error leaves the previous generation intact.
 */
class OutputTransaction {
public:
    explicit OutputTransaction(fs::path destination) : final_(std::move(destination)) {}
    ~OutputTransaction() {
        std::error_code ec;
        if (stageCreated_ && !stage_.empty()) fs::remove_all(stage_, ec);
        if (backupMoved_ && !committed_) {
            ec.clear();
            if (!fs::exists(final_, ec)) {
                ec.clear();
                fs::rename(backup_, final_, ec);
            }
        }
        if (!backup_.empty() && committed_) {
            ec.clear();
            fs::remove_all(backup_, ec);
        }
    }

    bool begin(std::string* why) {
        std::error_code ec;
        final_ = fs::absolute(final_, ec).lexically_normal();
        if (ec) { *why = "cannot resolve output destination: " + ec.message(); return false; }
        // '--out .' or 'out/' normalizes with a trailing separator: its filename()
        // is empty and parent_path() is the destination itself, which put the
        // stage INSIDE the output tree and copied it into itself without end.
        if (final_.filename().empty() && final_.has_parent_path()) final_ = final_.parent_path();
        if (final_.filename().empty() || final_ == final_.root_path()) {
            *why = "output destination must name a directory below the filesystem root";
            return false;
        }
        const fs::path parent = final_.parent_path();
        fs::create_directories(parent, ec);
        if (ec) { *why = "cannot create output parent: " + ec.message(); return false; }
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        const std::string suffix = ".javaconv-" + std::to_string(stamp);
        stage_ = parent / (final_.filename().string() + suffix + ".stage");
        backup_ = parent / (final_.filename().string() + suffix + ".backup");
        if (fs::exists(stage_, ec) || fs::exists(backup_, ec)) {
            *why = "transaction staging name already exists";
            return false;
        }
        const fs::file_status st = fs::symlink_status(final_, ec);
        const bool exists = !ec && st.type() != fs::file_type::not_found;
        if (ec != std::errc::no_such_file_or_directory && ec) {
            *why = "cannot inspect output destination: " + ec.message();
            return false;
        }
        if (exists && (fs::is_symlink(st) || !fs::is_directory(st))) {
            *why = "output destination must be a real directory, not a symlink or file";
            return false;
        }
        fs::create_directory(stage_, ec);
        if (ec) { *why = "cannot create transaction stage: " + ec.message(); return false; }
        stageCreated_ = true;
        if (exists && !copyTree(final_, stage_, why)) return false;
        return true;
    }

    const fs::path& stageRoot() const { return stage_; }

    bool commit(std::string* why) {
        std::error_code ec;
        const fs::file_status st = fs::symlink_status(final_, ec);
        const bool hadDestination = !ec && st.type() != fs::file_type::not_found;
        if (ec != std::errc::no_such_file_or_directory && ec) {
            *why = "cannot re-inspect output destination before publish: " + ec.message();
            return false;
        }
        if (hadDestination) {
            if (fs::is_symlink(st) || !fs::is_directory(st)) {
                *why = "output destination changed to a non-directory before publish";
                return false;
            }
            fs::rename(final_, backup_, ec);
            if (ec) { *why = "cannot move previous output generation aside: " + ec.message(); return false; }
            backupMoved_ = true;
        }
        fs::rename(stage_, final_, ec);
        if (ec) {
            const std::string publishError = ec.message();
            if (backupMoved_) {
                ec.clear();
                fs::rename(backup_, final_, ec);
                if (!ec) backupMoved_ = false;
            }
            *why = "cannot publish staged output generation: " + publishError +
                   (backupMoved_ ? "; previous generation remains at " + backup_.string() : "");
            return false;
        }
        stage_.clear();
        stageCreated_ = false;
        committed_ = true;
        return true;
    }

private:
    static bool copyTree(const fs::path& source, const fs::path& destination, std::string* why) {
        std::error_code ec;
        fs::recursive_directory_iterator it(source, fs::directory_options::none, ec), end;
        if (ec) { *why = "cannot enumerate previous output tree: " + ec.message(); return false; }
        while (it != end) {
            const fs::path entryPath = it->path();
            const fs::path rel = fs::relative(entryPath, source, ec);
            if (ec || !safeManifestPath(rel.generic_string())) {
                *why = "unsafe path in previous output tree";
                return false;
            }
            const fs::file_status st = it->symlink_status(ec);
            if (ec) { *why = "cannot inspect previous output entry: " + ec.message(); return false; }
            if (fs::is_symlink(st)) {
                *why = "refusing symlink in previous output tree: " + entryPath.string();
                return false;
            }
            const fs::path target = destination / rel;
            if (fs::is_directory(st)) {
                fs::create_directories(target, ec);
            } else if (fs::is_regular_file(st)) {
                fs::create_directories(target.parent_path(), ec);
                if (!ec) fs::copy_file(entryPath, target, fs::copy_options::overwrite_existing, ec);
                if (!ec) {
                    const auto sourceTime = fs::last_write_time(entryPath, ec);
                    if (!ec) fs::last_write_time(target, sourceTime, ec);
                }
            } else {
                *why = "refusing special file in previous output tree: " + entryPath.string();
                return false;
            }
            if (ec) { *why = "cannot stage previous output entry '" + rel.generic_string() + "': " + ec.message(); return false; }
            it.increment(ec);
            if (ec) { *why = "cannot enumerate previous output tree: " + ec.message(); return false; }
        }
        return true;
    }

    fs::path final_, stage_, backup_;
    bool stageCreated_ = false;
    bool backupMoved_ = false;
    bool committed_ = false;
};

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
                  << "  types=" << cm_count(&model, "type") << " promote=" << cm_count(&model, "promote")
                  << " binop=" << cm_count(&model, "binop") << " unop=" << cm_count(&model, "unop")
                  << " cast=" << cm_count(&model, "cast") << " widen=" << cm_count(&model, "widen")
                  << " rename=" << cm_count(&model, "rename") << " qualify=" << cm_count(&model, "qualify")
                  << " compound=" << cm_count(&model, "compound") << " ret=" << cm_count(&model, "ret")
                  << " unsupported=" << cm_count(&model, "unsupported") << "\n";
        // Required primitive operator laws and model-driven identifier maps.
        static const char* const need[] = {"+", "-", "*", "/", "%", "<<", ">>", ">>>"};
        int missing = 0;
        for (const char* op : need)
            for (const char* cls : {"int", "long"})
                if (!cm_find(&model, "binop", cls, op, nullptr)) { std::cerr << "missing binop " << cls << " " << op << "\n"; missing++; }
        static const char* const floatOps[] = {"+", "-", "*", "/", "%", "<", "<=", ">", ">=", "==", "!="};
        for (const char* op : floatOps)
            if (!cm_find(&model, "binop", "float", op, nullptr)) { std::cerr << "missing float binop " << op << "\n"; missing++; }
        for (const char* op : {"+", "-"})
            if (!cm_find(&model, "unop", "float", op, nullptr)) { std::cerr << "missing float unop " << op << "\n"; missing++; }
        const cm_rec* floatType = cm_find(&model, "type", "float", nullptr, nullptr);
        if (!floatType || std::string(floatType->f[1]) != "double") {
            std::cerr << "missing Java float storage spelling (double)\n"; missing++;
        }
        if (!cm_find(&model, "promote", "float", nullptr, nullptr)) { std::cerr << "missing float unary promotion\n"; missing++; }
        if (!cm_find(&model, "incr", "float", nullptr, nullptr)) { std::cerr << "missing float increment law\n"; missing++; }
        for (const char* op : {"+", "-", "*", "/", "%"})
            if (!cm_find(&model, "compound", "float", op, nullptr)) { std::cerr << "missing float compound " << op << "\n"; missing++; }
        if (!cm_find(&model, "widen", "int", "float", nullptr) ||
            !cm_find(&model, "widen", "long", "float", nullptr) ||
            !cm_find(&model, "widen", "float", "double", nullptr)) {
            std::cerr << "missing float widening law\n"; missing++;
        }
        if (!cm_find(&model, "cast", "double", "float", nullptr) ||
            !cm_find(&model, "cast", "float", "int", nullptr) ||
            !cm_find(&model, "cast", "float", "long", nullptr)) {
            std::cerr << "missing binary32 cast law\n"; missing++;
        }
        static const char* const numericTypes[] = {"byte", "short", "char", "int", "long", "float", "double"};
        for (const char* from : numericTypes)
            for (const char* to : numericTypes)
                if (!cm_find(&model, "cast", from, to, nullptr)) {
                    std::cerr << "missing primitive cast " << from << " -> " << to << "\n"; missing++;
                }
        if (!cm_find(&model, "tostr", "float", nullptr, nullptr)) { std::cerr << "missing float text law\n"; missing++; }
        if (!cm_find(&model, "rename", "next", "value", nullptr) ||
            !cm_find(&model, "rename", "close", "all", nullptr)) {
            std::cerr << "missing identifier mapping law\n"; missing++;
        }
        for (const char* name : {"close", "connect", "send"})
            if (!cm_find(&model, "qualify", name, nullptr, nullptr)) { std::cerr << "missing built-in qualification " << name << "\n"; missing++; }
        cm_free(&model);
        return missing ? 1 : 0;
    }

    std::vector<InputFile> inputs;  // sorted, immutable source snapshot
    fs::path outRoot;
    fs::path publishRoot;
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

    // Convert to a fixpoint. Pass 1 indexes the signatures of every unit still in
    // the project; pass 2 converts against that index. A unit that fails closed
    // emits nothing, so the types it declares do not exist in the output. A unit
    // that converted against those signatures would name members (overload
    // signature names, project-only types) that nothing defines. Each round
    // therefore drops the failed units from the index and converts the remaining
    // units again: their references to the dropped types now resolve the way a
    // reference to any non-project type does (the model and the lib/java tier),
    // or fail closed. The loop ends when a round adds no failure; it is bounded
    // by the number of units because every extra round removes at least one.
    // The /lib unit names the SLeeLa loader resolves (file stems). A reference to a
    // non-project type outside this set would not resolve, so the translator fails
    // it closed. No /lib directory (an installed converter without the library):
    // the check is off and the report says so.
    std::set<std::string> libraryUnits;
    std::string libraryFingerprint;
    {
        const fs::path lib = root / "lib";
        std::error_code lec;
        if (fs::is_directory(lib, lec)) {
            for (fs::recursive_directory_iterator li(lib, lec), lend; !lec && li != lend; li.increment(lec)) {
                std::error_code tec;
                if (li->is_regular_file(tec) && li->path().extension() == ".sleela")
                    libraryUnits.insert(li->path().stem().string());
            }
            if (lec) { std::cerr << "javaconv: cannot walk '" << lib.string() << "': " << lec.message() << "\n"; cm_free(&model); return 2; }
        }
        for (const auto& u : libraryUnits) { libraryFingerprint += u; libraryFingerprint.push_back('\n'); }
    }
    auto optionsFor = [&](const InputFile& in) {
        javaconv::Options o;
        o.sourceName = in.rel.generic_string();
        o.jdkVersion = c.version;
        o.author = c.author;
        o.singleThread = c.singleThread;
        o.checkedArrays = !c.uncheckedSizes;
        o.bundle = c.bundle || !c.file.empty();
        if (c.split) o.bundle = false;
        o.bundleName = in.path.stem().string();
        if (!libraryUnits.empty()) o.libraryUnits = &libraryUnits;
        return o;
    };
    std::vector<javaconv::Result> results(inputs.size());
    std::vector<bool> dropped(inputs.size(), false);
    std::vector<int> failedRound(inputs.size(), 0);
    for (int round = 1;; round++) {
        javaconv::SymbolTable table;
        for (size_t i = 0; i < inputs.size(); i++)
            if (!dropped[i]) javaconv::collect(inputs[i].source, model, optionsFor(inputs[i]), table);
        bool newFailure = false;
        for (size_t i = 0; i < inputs.size(); i++) {
            if (dropped[i]) continue;
            results[i] = javaconv::convert(inputs[i].source, model, optionsFor(inputs[i]), &table);
            if (!results[i].ok() || results[i].files.empty()) {
                dropped[i] = true;
                failedRound[i] = round;
                newFailure = true;
                if (round > 1)
                    for (auto& d : results[i].diagnostics)
                        d.message += " (round " + std::to_string(round) +
                                     ": after units that failed closed were removed from the project index)";
            }
        }
        if (!newFailure) break;
    }

    // Work on a complete staged generation. All existing output, including
    // unowned user files, is preserved in the stage; the destination is untouched
    // until every output, manifest, provenance file and report has been written.
    publishRoot = outRoot;
    OutputTransaction transaction(outRoot);
    std::string transactionError;
    if (!transaction.begin(&transactionError)) {
        std::cerr << "javaconv: " << transactionError << "\n";
        cm_free(&model);
        return 2;
    }
    outRoot = transaction.stageRoot();

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
    for (size_t idx = 0; idx < inputs.size(); idx++) {
        const InputFile& in = inputs[idx];
        FileOutcome fo;
        fo.rel = in.rel.generic_string();
        const javaconv::Result& r = results[idx];
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

    // Deterministic provenance: exact model bytes, immutable input snapshots,
    // converter source files, and emitted output bytes. No timestamps or
    // absolute checkout paths enter the manifest.
    std::string modelBytes;
    if (!readFile(modelPath, &modelBytes)) {
        std::cerr << "javaconv: cannot read conversion model for provenance: " << modelPath.string() << "\n";
        cm_free(&model); return 2;
    }
    const std::vector<std::string> converterPaths = {
        "sleelactl/convert/java_lex.c", "sleelactl/convert/java_lex.h",
        "sleelactl/convert/conv_model.c", "sleelactl/convert/conv_model.h",
        "sleelactl/convert/java_to_sleela.cpp", "sleelactl/convert/java_to_sleela.h",
        "sleelactl/convert/javaconv_main.cpp"
    };
    std::vector<std::pair<std::string,std::string>> converterDigests, outputDigests;
    std::string converterFingerprint, sourceFingerprint, outputFingerprint;
    bool converterSourcesComplete = true;
    for (const auto& rel : converterPaths) {
        std::string bytes;
        if (!readFile(root / rel, &bytes)) {
            // Installed distributions may ship the converter binary and model
            // without its C++ sources. Record the limitation instead of making
            // conversion itself depend on a source checkout.
            converterSourcesComplete = false;
            continue;
        }
        const std::string digest = sha256Hex(bytes);
        converterDigests.push_back({rel,digest});
        converterFingerprint += rel; converterFingerprint.push_back('\0'); converterFingerprint += digest; converterFingerprint.push_back('\n');
    }
    for (const auto& in : inputs) {
        const std::string rel = in.rel.generic_string(), digest = sha256Hex(in.source);
        sourceFingerprint += rel; sourceFingerprint.push_back('\0'); sourceFingerprint += digest; sourceFingerprint.push_back('\n');
    }
    for (const auto& rel : emittedRel) {
        std::string bytes;
        if (!safeManifestPath(rel) || !readFile(outRoot / rel, &bytes)) {
            std::cerr << "javaconv: cannot read emitted output for provenance: " << rel << "\n";
            cm_free(&model); return 2;
        }
        const std::string digest = sha256Hex(bytes);
        outputDigests.push_back({rel,digest});
        outputFingerprint += rel; outputFingerprint.push_back('\0'); outputFingerprint += digest; outputFingerprint.push_back('\n');
    }
    std::ostringstream provenance;
    provenance << "{\n"
      << "  \"schema\": \"sleelactl.javaconv.provenance.v1\",\n"
      << "  \"jdkVersionLabel\": \"" << jsonEscape(c.version) << "\",\n"
      << "  \"jdkIdentity\": \"source-snapshot-digest; release authenticity is not inferred from the label\",\n"
      << "  \"model\": {\"name\": \"" << jsonEscape(model.name) << "\", \"revision\": \"" << jsonEscape(model.rev)
      << "\", \"sha256\": \"" << sha256Hex(modelBytes) << "\"},\n"
      << "  \"converterSourceIdentity\": \"available working-tree source files; compiled binary identity is not recorded\",\n"
      << "  \"converterSourceComplete\": " << (converterSourcesComplete ? "true" : "false") << ",\n"
      << "  \"converterSourceSha256\": \"" << sha256Hex(converterFingerprint) << "\",\n"
      << "  \"inputSnapshotSha256\": \"" << sha256Hex(sourceFingerprint) << "\",\n"
      << "  \"outputSnapshotSha256\": \"" << sha256Hex(outputFingerprint) << "\",\n"
      << "  \"libraryUnits\": {\"count\": " << libraryUnits.size() << ", \"sha256\": \"" << sha256Hex(libraryFingerprint)
      << "\", \"identity\": \"sorted .sleela file stems under <repo>/lib, one per line; non-project types must be one of them\"},\n"
      << "  \"options\": {\"bundle\": " << ((c.bundle || !c.file.empty()) && !c.split ? "true" : "false")
      << ", \"singleThread\": " << (c.singleThread ? "true" : "false")
      << ", \"checkedArrays\": " << (c.uncheckedSizes ? "false" : "true")
      << ", \"author\": \"" << jsonEscape(c.author) << "\"},\n"
      << "  \"converterSources\": [";
    for (size_t i=0;i<converterDigests.size();++i) {
        if (i) provenance << ",";
        provenance << "\n    {\"path\": \"" << jsonEscape(converterDigests[i].first) << "\", \"sha256\": \"" << converterDigests[i].second << "\"}";
    }
    if (!converterDigests.empty()) provenance << "\n  ";
    provenance << "],\n  \"inputs\": [";
    for (size_t i=0;i<inputs.size();++i) {
        if (i) provenance << ",";
        provenance << "\n    {\"path\": \"" << jsonEscape(inputs[i].rel.generic_string()) << "\", \"sha256\": \"" << sha256Hex(inputs[i].source) << "\"}";
    }
    if (!inputs.empty()) provenance << "\n  ";
    provenance << "],\n  \"outputs\": [";
    for (size_t i=0;i<outputDigests.size();++i) {
        if (i) provenance << ",";
        provenance << "\n    {\"path\": \"" << jsonEscape(outputDigests[i].first) << "\", \"sha256\": \"" << outputDigests[i].second << "\"}";
    }
    if (!outputDigests.empty()) provenance << "\n  ";
    provenance << "]\n}\n";
    std::string provenanceError;
    if (!writeIfChanged(outRoot / "CONVERSION-PROVENANCE.json", provenance.str(), &provenanceError)) {
        std::cerr << "javaconv: " << provenanceError << "\n"; cm_free(&model); return 2;
    }

    // ---- report ----
    std::error_code ec;
    fs::create_directories(outRoot, ec);
    {
        std::ostringstream j;
        j << "{\n  \"provenance\": \"CONVERSION-PROVENANCE.json\",\n  \"model\": \"" << jsonEscape(model.name) << " " << jsonEscape(model.rev) << "\",\n"
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
          << "- failed closed: " << failed << "\n"
          << "- deterministic provenance: [CONVERSION-PROVENANCE.json](CONVERSION-PROVENANCE.json)\n\n"
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

    if (!transaction.commit(&transactionError)) {
        std::cerr << "javaconv: " << transactionError << "\n";
        cm_free(&model);
        return 2;
    }
    if (!c.quiet) {
        std::cout << "javaconv: model " << model.name << " " << model.rev << ", JDK " << c.version << "\n"
                  << "  source files " << outcomes.size() << ", converted " << okFiles << " (" << emitted << " SLeeLa files), failed closed " << failed << "\n"
                  << "  output " << publishRoot.string() << " (CONVERSION-REPORT.md / .json)\n";
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
