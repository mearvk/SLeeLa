// ===========================================================================
// driver.cpp -- Sleelvac™ command-line compiler/runtime front end.
// ===========================================================================
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "compiler.h"
#include "artifact.h"
#include "originate.h"
#include "wikigen.h"
#include "native_api.h"
#include "chemistry_api.h"
#include "financial_api.h"
#include "version.h"
#include "library_index.h"
#include "../../registry/sleela_registry.h"
#include "../annotation/AnnotationRuntime.hpp"
#include "../catalog/sheet_catalog.h"
#include "../xclass/xclass_loader.h"
#include "../langin/langin.h"
#include "../nordshrift/sleela_emit.h"
#include "http_server_cli.h"
#include "../core/sleela_audio_bridge.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <chrono>
#include <cerrno>
#include <cstring>
#if defined(_WIN32)
#  include <process.h>
#else
#  include <sys/wait.h>
#endif
extern "C" {
#include "../core/sleela_core.h"
#include "../core/sleela_memmgr.h"
#include "../core/sleela_terminal.h"
#include "../core/sleela_design_activity.h"
#include "../../runtime/java_runtime_bridge.h"
}
#if defined(_WIN32)
#  include <windows.h>
#elif defined(__APPLE__)
#  include <mach-o/dyld.h>
#  include <climits>
#else
#  include <unistd.h>
#  include <climits>
#endif
namespace fs=std::filesystem;

// Absolute path of the running executable, or empty if it cannot be determined.
// Used to make an *installed* binary find its support files (the /lib tree, the
// SHA-256 manifest + verifier, the SHEET.sheet catalog) relative to its own
// install location rather than only relative to the current working directory.
static fs::path executablePath(){
    std::error_code ec;
#if defined(_WIN32)
    wchar_t buf[MAX_PATH];
    DWORD n=GetModuleFileNameW(nullptr,buf,MAX_PATH);
    if(n==0||n>=MAX_PATH) return fs::path();
    fs::path p(std::wstring(buf,buf+n));
    fs::path c=fs::weakly_canonical(p,ec); return ec?p:c;
#elif defined(__APPLE__)
    char buf[PATH_MAX]; uint32_t size=sizeof(buf);
    if(_NSGetExecutablePath(buf,&size)!=0) return fs::path();
    fs::path p(buf);
    fs::path c=fs::weakly_canonical(p,ec); return ec?p:c;
#else
    char buf[PATH_MAX];
    ssize_t n=::readlink("/proc/self/exe",buf,sizeof(buf)-1);
    if(n<=0) return fs::path();
    buf[n]='\0';
    fs::path p(buf);
    fs::path c=fs::weakly_canonical(p,ec); return ec?p:c;
#endif
}

// Derive the SLeeLa runtime root from the installed layout, where the binary
// lives at <prefix>/bin/<cmd> and its support tree at <prefix>/lib/sleela. A
// directory is accepted only when it carries the verifier the gate needs, so a
// non-installed (build-tree) binary falls through to the env var / upward
// search instead of guessing. Returns empty when no install root is found.
static fs::path installRootFromExecutable(){
    fs::path exe=executablePath();
    if(exe.empty()) return fs::path();
    std::error_code ec;
    fs::path bindir=exe.parent_path();                 // <prefix>/bin
    fs::path prefix=bindir.parent_path();               // <prefix>
    // Primary installed layout: <prefix>/lib/sleela.
    fs::path candidates[]={
        prefix/"lib"/"sleela",
        // Fallbacks for less conventional layouts (bin and runtime side by side,
        // or the runtime tree sitting next to the executable).
        bindir/".."/"lib"/"sleela",
        bindir/"sleela",
    };
    for(const auto& c:candidates){
        fs::path d=fs::weakly_canonical(c,ec); if(ec){ec.clear();d=c;}
        if(fs::is_directory(d,ec) &&
           fs::is_regular_file(d/"tools"/"verify-before-execution.py",ec)) return d;
        ec.clear();
    }
    return fs::path();
}
// The runtime (`sleela`) and the compiler (`sleelvac`) share this front-end
// source, but each binary is built as an INDEPENDENT executable with its
// product identity baked in at compile time (SLEELA_PRODUCT_SLEELVAC is defined
// only for the sleelvac object). `sleela version` therefore always reports the
// SLeeLa Language/Runtime, independent of argv[0], launchers, symlinks, or
// aliases. Both banners carry the same release number.
// The shared release number for both tools.
static const char* kReleaseVersion="0.3.29-dev";
// Both banners derive the supported source-syntax range from the single source
// of truth in version.h (minSupportedSyntax()/maxSupportedSyntax()), so the
// reported range always matches what the toolchain actually drives.
static std::string sleelaVersionBanner(){
    return std::string("SLeeLa\u2122 Language/Runtime ")+kReleaseVersion
         +" (supported source syntax "+sleela::minSupportedSyntax().str()
         +" \u2013 "+sleela::maxSupportedSyntax().str()
         +"; native C/C++ Core VM)";
}
static std::string sleelvacVersionBanner(){
    return std::string("Sleelvac\u2122 ")+kReleaseVersion
         +" (SLeeLa compiler; compiles source syntax "+sleela::minSupportedSyntax().str()
         +" \u2013 "+sleela::maxSupportedSyntax().str()
         +"; executable native math/physics/economics/chemistry/financial modules;"
          " persistent .sleela Core artifacts; .xclass input; JVM-family langin input:"
          " Java/Kotlin/Scala/Groovy/Clojure; Nordshrift round-trip"
          " (SLeeLa->Nordshrift->back); OS Defender provisioning; SHA-256 execution gate)";
}
// Compile-time product identity. The sleelvac object is compiled with
// -DSLEELA_PRODUCT_SLEELVAC; every other build is the runtime (`sleela`).
#if defined(SLEELA_PRODUCT_SLEELVAC)
static const bool kBuiltAsCompiler=true;
#else
static const bool kBuiltAsCompiler=false;
#endif
static bool readFile(const std::string& path,std::string& out);
static int runLangin(const std::vector<std::string>&paths);
static bool hasExt(const std::string& path,const std::string& ext){return path.size()>=ext.size()&&path.compare(path.size()-ext.size(),ext.size(),ext)==0;}
static std::string shellQuote(const std::string&s){
#ifdef _WIN32
    std::string q="\"";for(char c:s){if(c=='\"')q+="\\\"";else q+=c;}return q+"\"";
#else
    std::string q="'";for(char c:s){if(c=='\'')q+="'\\''";else q+=c;}return q+"'";
#endif
}
static int runCommand(const std::string&cmd){std::cout<<"[defender] $ "<<cmd<<"\n";return std::system(cmd.c_str())==0?0:1;}
// ---------------------------------------------------------------------------
// Defender opt-in / safety gates.
//
// The `defender` subcommand can download a remote source tree and install a
// privileged (kernel) driver. Because that is a supply-chain and privilege
// escalation surface, every network/build/install action is gated behind an
// EXPLICIT opt-in and the downloaded payload is integrity-checked before use.
//
//  * SLEELA_DEFENDER_OPTIN=1 (or --allow-defender) must be set to permit any
//    action that fetches, builds, or installs the defender. Without it, the
//    only permitted action is `detect`, which contacts nothing.
//  * SLEELA_DEFENDER_SHA256=<hex> (or --sha256 <hex>) must be set to the
//    expected SHA-256 of the downloaded archive; the fetch fails closed if the
//    hash is missing or does not match. There is no "trust on first use".
//  * SLEELA_DEFENDER_ALLOW_ROOT=1 (or --allow-root) must be set before any
//    step that elevates privileges (sudo / pnputil). Elevation is never
//    performed implicitly.
// ---------------------------------------------------------------------------
struct DefenderOpts{bool optin=false;bool allowRoot=false;std::string sha256;};
static bool envFlag(const char*name){const char*v=std::getenv(name);return v&&(std::string(v)=="1"||std::string(v)=="true"||std::string(v)=="yes");}
static std::string toLowerHex(std::string s){for(char&c:s)c=(char)std::tolower((unsigned char)c);return s;}
static std::string readSha256Env(){const char*v=std::getenv("SLEELA_DEFENDER_SHA256");return v?toLowerHex(v):std::string();}
static bool sha256OfFile(const fs::path&path,std::string&hexOut){
    // Delegate to a trusted external hasher so we never re-implement crypto here.
#ifdef _WIN32
    std::string cmd="certutil -hashfile "+shellQuote(path.string())+" SHA256";
#else
    std::string cmd="sha256sum "+shellQuote(path.string());
#endif
    std::string tmpName=(fs::temp_directory_path()/("sleela-defender-hash-"+std::to_string((unsigned long long)std::rand())+".txt")).string();
    if(std::system((cmd+" > "+shellQuote(tmpName)+" 2>"+
#ifdef _WIN32
        "NUL"
#else
        "/dev/null"
#endif
        ).c_str())!=0){std::error_code ec;fs::remove(tmpName,ec);return false;}
    std::ifstream f(tmpName);std::string content((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());f.close();std::error_code ec;fs::remove(tmpName,ec);
    // Extract the first 64-hex-char token from the tool output.
    std::string cur;for(char c:content){bool hex=(c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F');if(hex){cur+=c;if(cur.size()==64){hexOut=toLowerHex(cur);return true;}}else{cur.clear();}}
    return false;
}
static bool verifyArchiveHash(const fs::path&archive,const std::string&expected){
    if(expected.empty()){std::cerr<<"sleelvac: defender: refusing to use downloaded archive without an expected SHA-256.\n  Provide one with --sha256 <hex> or SLEELA_DEFENDER_SHA256=<hex>.\n";return false;}
    if(expected.size()!=64){std::cerr<<"sleelvac: defender: expected SHA-256 must be 64 hex characters.\n";return false;}
    std::string actual;if(!sha256OfFile(archive,actual)){std::cerr<<"sleelvac: defender: unable to compute SHA-256 of downloaded archive.\n";return false;}
    if(actual!=expected){std::cerr<<"sleelvac: defender: downloaded archive SHA-256 mismatch.\n  expected: "<<expected<<"\n  actual:   "<<actual<<"\nAborting; the payload is not trusted.\n";return false;}
    std::cout<<"[defender] archive SHA-256 verified: "<<actual<<"\n";return true;
}
// Locate the repository root by walking up from `start` until a directory
// containing tools/verify-before-execution.py is found. This lets `sleela run`
// (and every other gated subcommand) work from any current working directory --
// e.g. from impl/ -- rather than only from the repo root. Returns an empty path
// if no such ancestor exists.
static fs::path findRepoRoot(const fs::path&start){
    std::error_code ec;

    // Prefer the explicitly configured SLeeLa installation/project root. This
    // is essential when a client project (for example Airport Tycoon) invokes
    // Sleelvac from its own edition directory, outside the SLeeLa checkout.
    // Only trust SLEELA_HOME when it contains the required verifier; otherwise
    // fall back to the normal upward search.
    if(const char* home=std::getenv("SLEELA_HOME"); home && *home){
        fs::path configured=fs::absolute(fs::path(home),ec);
        if(ec){ec.clear();configured=fs::path(home);}
        if(fs::is_directory(configured,ec) &&
           fs::is_regular_file(configured/"tools"/"verify-before-execution.py",ec)){
            fs::path canonical=fs::weakly_canonical(configured,ec);
            return ec ? configured : canonical;
        }
        ec.clear();
    }

    // Next, prefer the installed layout derived from the executable's own
    // location (<prefix>/bin/<cmd> -> <prefix>/lib/sleela). This lets an
    // installed `sleela`/`sleelvac` find its support tree from the final install
    // location regardless of the current working directory or environment, which
    // is what a user expects when running the installed command rather than from
    // the git checkout. SLEELA_HOME above still wins as an explicit override.
    if(fs::path installed=installRootFromExecutable(); !installed.empty())
        return installed;

    fs::path dir=fs::absolute(start,ec);
    if(ec){ec.clear();dir=start;}
    for(;;){
        if(fs::is_regular_file(dir/"tools"/"verify-before-execution.py",ec)){
            fs::path canonical=fs::weakly_canonical(dir,ec);
            return ec ? dir : canonical;
        }
        ec.clear();
        fs::path parent=dir.parent_path();
        if(parent.empty()||parent==dir)break;
        dir=parent;
    }
    return fs::path();
}

// Resolve the authoritative /lib source-library root for the current invocation,
// honoring (in order): $SLEELA_LIB, the installed <prefix>/lib/sleela/lib, the
// SLeeLa root found by findRepoRoot (SLEELA_HOME or the checkout), and finally
// an empty string so Index::discover falls back to its cwd-relative search.
// Centralizing this keeps the import validator (compiler.cpp) and the source
// unit loader (loadReferencedLibraryUnits) in agreement about where /lib is.
static std::string resolveLibraryRoot(){
    if(const char* configured=std::getenv("SLEELA_LIB"); configured&&*configured) return configured;
    std::error_code ec;
    if(fs::path installed=installRootFromExecutable(); !installed.empty()){
        fs::path lib=installed/"lib";
        if(fs::is_directory(lib,ec)) return lib.string();
        ec.clear();
    }
    fs::path repo=findRepoRoot(fs::current_path());
    if(!repo.empty()){
        fs::path lib=repo/"lib";
        if(fs::is_directory(lib,ec)) return lib.string();
        ec.clear();
    }
    return std::string();
}
static int verifyBeforeExecution(const fs::path&cwd){
    // Resolve the SLeeLa root first so a configured installation works even
    // when the caller is a separate client project and has no local verifier.
    fs::path root=findRepoRoot(cwd);
    if(root.empty())root=cwd;

    const char* env=std::getenv("SLEELA_SHA256_MANIFEST");
    fs::path manifest=(env&&*env)?fs::path(env):(root/"security"/"sha256-manifest.json");
    if(!manifest.is_absolute())manifest=root/manifest;

    fs::path tool=root/"tools"/"verify-before-execution.py";
    std::error_code ec;
    if(!fs::is_regular_file(tool,ec)){
        std::cerr<<"sleelvac: verification tool not found: "<<tool
                 <<"\n  (set SLEELA_HOME to the SLeeLa root, or search upward from "<<cwd<<")\n";
        return 1;
    }
    ec.clear();
    if(!fs::is_regular_file(manifest,ec)){
        std::cerr<<"sleelvac: trusted SHA-256 manifest not found: "<<manifest
                 <<"\n  (set SLEELA_SHA256_MANIFEST or provide security/sha256-manifest.json under the selected SLeeLa root)\n";
        return 1;
    }
#ifdef _WIN32
    std::string cmd="py -3 "+shellQuote(tool.string())+" --manifest "+shellQuote(manifest.string())+" --root "+shellQuote(root.string());
#else
    std::string cmd="python3 "+shellQuote(tool.string())+" --manifest "+shellQuote(manifest.string())+" --root "+shellQuote(root.string());
#endif
    std::cout<<"[security] SHA-256 verification required before build/execution/diagnostics\n";
    return runCommand(cmd);
}
static const char* defenderRepo(){
#ifdef _WIN32
    return "https://github.com/mearvk/Windows.Admin.Defender";
#elif defined(__APPLE__)
    return "unsupported-on-macos";
#else
    return "https://github.com/mearvk/Linux.Admin.Defender";
#endif
}
static const char* defenderName(){
#ifdef _WIN32
    return "Windows.Admin.Defender";
#elif defined(__APPLE__)
    return "MacOS.Admin.Defender";
#else
    return "Linux.Admin.Defender";
#endif
}
static bool defenderPlatformSupported(){
#if defined(_WIN32) || defined(__linux__)
    return true;
#else
    return false;
#endif
}
static bool findDownloader(std::string&tool){
#ifdef _WIN32
    if(std::system("where curl.exe >nul 2>&1")==0){tool="curl.exe";return true;}
    if(std::system("where wget.exe >nul 2>&1")==0){tool="wget.exe";return true;}
#else
    if(std::system("command -v curl >/dev/null 2>&1")==0){tool="curl";return true;}
    if(std::system("command -v wget >/dev/null 2>&1")==0){tool="wget";return true;}
#endif
    return false;
}
static int defenderFetch(const fs::path&base,const DefenderOpts&opts){
    if(!defenderPlatformSupported()){std::cerr<<"sleelvac: defender: no privileged Defender backend is implemented for macOS.\n";return 1;}
    std::string dl;if(!findDownloader(dl)){std::cerr<<"sleelvac: defender: neither curl nor wget is installed\n";return 1;}
    fs::create_directories(base);fs::path archive=base/(std::string(defenderName())+".zip");fs::path source=base/defenderName();
    if(fs::exists(source)){std::cout<<"[defender] source already exists: "<<source<<"\n";return 0;}
    std::string url=std::string("https://codeload.github.com/")+((std::string(defenderName())=="Windows.Admin.Defender")?"mearvk/Windows.Admin.Defender":"mearvk/Linux.Admin.Defender")+"/zip/refs/heads/main";
    std::cout<<"[defender] fetching remote source: "<<url<<"\n";
    if(dl.find("curl")!=std::string::npos) {if(runCommand(dl+" -L --fail --silent --show-error "+shellQuote(url)+" -o "+shellQuote(archive.string())))return 1;}
    else {if(runCommand(dl+" -q "+shellQuote(url)+" -O "+shellQuote(archive.string())))return 1;}
    // Fail closed unless the downloaded archive matches the expected hash.
    if(!verifyArchiveHash(archive,opts.sha256)){std::error_code ec;fs::remove(archive,ec);return 1;}
#ifdef _WIN32
    std::string ps="Expand-Archive -LiteralPath "+shellQuote(archive.string())+" -DestinationPath "+shellQuote(base.string())+" -Force";
    if(runCommand("powershell.exe -NoProfile -NonInteractive -Command "+shellQuote(ps)))return 1;
#else
    if(runCommand("unzip -q -o "+shellQuote(archive.string())+" -d "+shellQuote(base.string())))return 1;
#endif
    fs::path extracted=base/(std::string(defenderName())+"-main");if(!fs::exists(extracted)){std::cerr<<"sleelvac: defender: expected source tree not found: "<<extracted<<"\n";return 1;}
    std::error_code ec;fs::remove(archive,ec);fs::rename(extracted,source,ec);if(ec){std::cerr<<"sleelvac: defender: cannot normalize source directory: "<<ec.message()<<"\n";return 1;}
    std::cout<<"[defender] fetched "<<defenderRepo()<<" -> "<<source<<"\n";return 0;
}
static int defenderBuild(const fs::path&source){
    if(!defenderPlatformSupported()){std::cerr<<"sleelvac: defender: no privileged Defender backend is implemented for macOS.\n";return 1;}
    if(!fs::exists(source)){std::cerr<<"sleelvac: defender: source not found: "<<source<<"\n";return 1;}
#ifdef _WIN32
    fs::path script=source/"build"/"build.ps1";if(!fs::exists(script)){std::cerr<<"sleelvac: defender: missing build/build.ps1\n";return 1;}
    return runCommand("powershell.exe -NoProfile -NonInteractive -File "+shellQuote(script.string()));
#else
    fs::path dir=source/"kernel"/"file-locker";if(!fs::exists(dir/"Makefile")){std::cerr<<"sleelvac: defender: missing kernel/file-locker/Makefile\n";return 1;}
    return runCommand("make -C "+shellQuote(dir.string()));
#endif
}
static int defenderInstall(const fs::path&source,const DefenderOpts&opts){
    if(!defenderPlatformSupported()){std::cerr<<"sleelvac: defender: no privileged Defender backend is implemented for macOS.\n";return 1;}
    if(!opts.allowRoot){
        std::cerr<<"sleelvac: defender: installation requires elevated privileges and is refused by default.\n"
                   "  This step would install a privileged/kernel driver on this machine.\n"
                   "  Re-run with --allow-root (or SLEELA_DEFENDER_ALLOW_ROOT=1) to authorize it explicitly.\n";
        return 1;
    }
#ifdef _WIN32
    fs::path inf=source/"source"/"WindowsAdminDefender.inf";if(!fs::exists(inf)){std::cerr<<"sleelvac: defender: missing WindowsAdminDefender.inf\n";return 1;}
    std::cerr<<"[defender] Windows installation requires an elevated terminal and a properly signed WDK driver package.\n";
    return runCommand("pnputil.exe /add-driver "+shellQuote(inf.string())+" /install");
#else
    fs::path dir=source/"kernel"/"file-locker";
    std::cerr<<"[defender] elevating with sudo to install a kernel module (authorized via --allow-root).\n";
    return runCommand("sudo make -C "+shellQuote(dir.string())+" install");
#endif
}
static void defenderUsage(){
    std::cerr<<"Usage:\n"
               "  sleela defender detect\n"
               "  sleela defender <fetch|build|install|provision> [directory] --allow-defender [options]\n"
               "\n"
               "Actions that fetch/build/install remote code require explicit opt-in:\n"
               "  --allow-defender             opt in to network/build/install actions\n"
               "                               (or SLEELA_DEFENDER_OPTIN=1)\n"
               "  --sha256 <hex>               expected SHA-256 of the downloaded archive; required\n"
               "                               for fetch/build/install/provision (or SLEELA_DEFENDER_SHA256)\n"
               "  --allow-root                 authorize the privileged install step (sudo / pnputil);\n"
               "                               never performed implicitly (or SLEELA_DEFENDER_ALLOW_ROOT=1)\n";
}
static int defenderCmd(int argc,char**argv){
    std::string action=argc>=3?argv[2]:"detect";
    // Parse action, optional directory, and the safety flags in any order.
    DefenderOpts opts;opts.optin=envFlag("SLEELA_DEFENDER_OPTIN");opts.allowRoot=envFlag("SLEELA_DEFENDER_ALLOW_ROOT");opts.sha256=readSha256Env();
    fs::path base=fs::temp_directory_path()/"sleela-defender";bool baseSet=false;
    for(int i=3;i<argc;i++){
        std::string a=argv[i];
        if(a=="--allow-defender"||a=="--optin")opts.optin=true;
        else if(a=="--allow-root")opts.allowRoot=true;
        else if(a=="--sha256"){if(i+1>=argc){std::cerr<<"sleelvac: defender: --sha256 requires a hex value\n";return 2;}opts.sha256=toLowerHex(argv[++i]);}
        else if(a.rfind("--sha256=",0)==0)opts.sha256=toLowerHex(a.substr(9));
        else if(a.rfind("--",0)==0){std::cerr<<"sleelvac: defender: unknown option '"<<a<<"'\n";defenderUsage();return 2;}
        else if(!baseSet){base=fs::path(a);baseSet=true;}
        else{std::cerr<<"sleelvac: defender: unexpected argument '"<<a<<"'\n";return 2;}
    }
    fs::path source=base/defenderName();
    // `detect` performs no network/build/install activity and needs no opt-in.
    if(action=="detect"){if(verifyBeforeExecution(fs::current_path()))return 1;std::cout<<"OS defender: "<<defenderName()<<"\nRepository: "<<defenderRepo()<<"\n";return 0;}
    if(action!="fetch"&&action!="build"&&action!="install"&&action!="provision"){std::cerr<<"sleelvac: unknown defender action '"<<action<<"'\n";defenderUsage();return 2;}
    // All remaining actions download/build/install remote code: require opt-in.
    if(!opts.optin){
        std::cerr<<"sleelvac: defender: '"<<action<<"' downloads and/or installs remote code and is disabled by default.\n"
                   "  Re-run with --allow-defender (or SLEELA_DEFENDER_OPTIN=1) to opt in.\n";
        defenderUsage();return 1;
    }
    if(action=="fetch"){if(verifyBeforeExecution(fs::current_path()))return 1;return defenderFetch(base,opts);}
    if(action=="build"){if(!fs::exists(source)&&defenderFetch(base,opts))return 1;if(verifyBeforeExecution(fs::current_path()))return 1;return defenderBuild(source);}
    if(action=="install"){if(!fs::exists(source)&&defenderFetch(base,opts))return 1;if(verifyBeforeExecution(fs::current_path()))return 1;if(defenderBuild(source))return 1;return defenderInstall(source,opts);}
    /*provision*/{if(defenderFetch(base,opts))return 1;if(verifyBeforeExecution(fs::current_path()))return 1;if(defenderBuild(source))return 1;return defenderInstall(source,opts);}
}
// ---------------------------------------------------------------------------
// Memory Manager wiring.
// SLeeLa has a process-wide managed-memory ceiling of 2 GiB. The configured
// value may be reduced to 256 MiB and is accepted from a config file, the
// SLEELA_MEMORY_MANAGER environment input, or the command line.
// Precedence: built-in default -> config file -> environment -> CLI.
static constexpr size_t kDefaultMemoryLimit = 2048ull*1024ull*1024ull;
static constexpr size_t kMinimumMemoryLimit = 256ull*1024ull*1024ull;
static constexpr size_t kMaximumMemoryLimit = 2048ull*1024ull*1024ull;

static bool parseByteSize(const std::string&in,size_t&out){
    if(in.empty())return false;
    std::string low=toLowerHex(in);
    if(low=="on"||low=="true"||low=="yes"||low=="1"){out=kDefaultMemoryLimit;return true;}
    size_t i=0;unsigned long long v=0;bool any=false;
    for(;i<in.size()&&std::isdigit((unsigned char)in[i]);++i){v=v*10ull+(unsigned)(in[i]-'0');any=true;}
    if(!any)return false;
    unsigned long long mult=1;
    if(i<in.size()){
        char c=(char)std::tolower((unsigned char)in[i]);
        if(c=='k')mult=1024ull;else if(c=='m')mult=1024ull*1024ull;else if(c=='g')mult=1024ull*1024ull*1024ull;
        else return false;
        ++i;if(i<in.size()&&(in[i]=='b'||in[i]=='B'))++i;
    }
    if(i!=in.size())return false;
    unsigned long long bytes=v*mult;
    if(bytes<kMinimumMemoryLimit||bytes>kMaximumMemoryLimit)return false;
    out=(size_t)bytes; return true;
}
static bool loadMemoryConfig(const std::string&path,size_t&limit,bool&enabled){
    std::ifstream f(path); if(!f)return false;
    std::string line;
    while(std::getline(f,line)){
        auto hash=line.find('#'); if(hash!=std::string::npos)line.erase(hash);
        auto eq=line.find('='); if(eq==std::string::npos)continue;
        std::string key=line.substr(0,eq), value=line.substr(eq+1);
        auto trim=[](std::string&s){while(!s.empty()&&std::isspace((unsigned char)s.front()))s.erase(s.begin());while(!s.empty()&&std::isspace((unsigned char)s.back()))s.pop_back();};
        trim(key);trim(value);
        if(key=="memory_manager"||key=="memory_limit"||key=="ram_limit"){
            std::string low=toLowerHex(value);
            if(low=="off"||low=="false"||low=="no"||low=="0"){enabled=false;continue;}
            size_t parsed=0;if(!parseByteSize(value,parsed)){std::cerr<<"sleela: invalid memory setting in "<<path<<": "<<value<<" (allowed: 256M..2G)\n";return false;}
            limit=parsed;enabled=true;
        }
    }
    return true;
}
static bool memoryManagerRequested(size_t&limit){
    const char*env=std::getenv("SLEELA_MEMORY_MANAGER");
    if(env&&*env){size_t l=0;if(parseByteSize(env,l)){limit=l;return true;}
        std::string e=toLowerHex(env);if(e=="0"||e=="off"||e=="false"||e=="no")return false;}
    return false;
}
static bool enableMemoryManager(size_t limit,bool announce){
    if(slmm_enable(limit)!=0){std::cerr<<"sleela: memory limit must be between 256 MiB and 2 GiB.\n";return false;}
    if(announce)std::cerr<<"[memory-manager] enabled (hard limit "<<slmm_limit()<<" bytes)\n";
    return true;
}
static void reportMemoryManager(){
    if(!slmm_is_enabled())return;
    char buf[512];slmm_format_report(buf,sizeof buf);std::cerr<<buf;
}
// ---------------------------------------------------------------------------
// Native executable runner.
//
// `sleela native [--] <program> [args...]` runs a *native* OS executable from
// the SLeeLa terminal, under a real pseudo-terminal (the same OS-aware PTY
// primitive the runtime uses), relaying its output and preserving its exit
// status. It honors the SHA-256 execution gate exactly like run/check/compile,
// and it turns the Memory Manager on for the duration (on-need) so a runaway
// native cannot silently exhaust host memory through the manager's allocations.
// ---------------------------------------------------------------------------
static void nativeUsage(){
    std::cerr<<"Usage:\n"
               "  sleela native [--config <file>] [--memory-manager[=<size>]] [--] <program> [args...]\n  sleela design-activity <science> <correctness> <reproducibility> <observability> <safety> <resource> <interoperability>\n"
               "  sleela exec   [--config <file>] [--memory-manager[=<size>]] [--] <program> [args...]\n"
               "\n"
               "Runs a native OS executable from the SLeeLa terminal under a real\n"
               "pseudo-terminal, relaying its output and preserving its exit status.\n"
               "--config selects a common properties/configuration file and exports\n"
               "its path to the child as SLEELA_CONFIG_FILE. Arguments after the\n"
               "executable are passed through unchanged, including numeric flags.\n"
               "The Memory Manager is enabled for the run; --memory-manager=<size> sets\n"
               "a hard byte limit (accepts K/M/G suffixes).\n";
}
static int nativeCmd(int argc,char**argv){
    size_t mmLimit=kDefaultMemoryLimit;bool mmExplicit=false;
    std::string configFile;
    std::vector<std::string> cmdArgs;
    bool endOpts=false;
    for(int i=2;i<argc;i++){
        std::string a=argv[i];
        if(!endOpts&&a=="--"){endOpts=true;continue;}
        if(!endOpts&&(a=="--config"||a=="-c")){
            if(i+1>=argc){std::cerr<<"sleela native: --config requires a file\n";return 2;}
            configFile=argv[++i];
        }
        else if(!endOpts&&a.rfind("--config=",0)==0){
            configFile=a.substr(9);
        }
        else if(!endOpts&&(a=="--memory-manager"||a=="--mm")){mmExplicit=true;mmLimit=0;}
        else if(!endOpts&&(a.rfind("--memory-manager=",0)==0||a.rfind("--mm=",0)==0)){
            std::string v=a.substr(a.find('=')+1);size_t l=0;
            if(!parseByteSize(v,l)){std::cerr<<"sleela native: invalid --memory-manager size '"<<v<<"'\n";return 2;}
            mmExplicit=true;mmLimit=l;
        }
        else if(!endOpts&&(a=="-h"||a=="--help")){nativeUsage();return 0;}
        else if(!endOpts&&a.rfind("--",0)==0){std::cerr<<"sleela native: unknown option '"<<a<<"'\n";nativeUsage();return 2;}
        else{endOpts=true;cmdArgs.push_back(a);}
    }
    if(cmdArgs.empty()){std::cerr<<"sleela native: no program given\n";nativeUsage();return 2;}
    if(!configFile.empty()){
        std::error_code ec;
        fs::path p=fs::absolute(configFile,ec);
        if(ec||!fs::is_regular_file(p)){
            std::cerr<<"sleela native: config file not found or not a regular file: "<<configFile<<"\n";
            return 2;
        }
        configFile=p.string();
    }
    if(verifyBeforeExecution(fs::current_path()))return 1;
    (void)mmExplicit;
    if(!enableMemoryManager(mmLimit,true))return 2;
    if(!configFile.empty()){
#ifdef _WIN32
        _putenv_s("SLEELA_CONFIG_FILE",configFile.c_str());
#else
        setenv("SLEELA_CONFIG_FILE",configFile.c_str(),1);
#endif
        std::cout<<"[native] config: "<<configFile<<"\n";
    }
    std::string command;
    for(size_t i=0;i<cmdArgs.size();++i){if(i)command+=' ';command+=shellQuote(cmdArgs[i]);}
    std::cout<<"[native] launching: "<<command<<"\n";
    unsigned cols=80,rows=24;
    if(const char*c=std::getenv("COLUMNS")){int v=std::atoi(c);if(v>0)cols=(unsigned)v;}
    if(const char*r=std::getenv("LINES")){int v=std::atoi(r);if(v>0)rows=(unsigned)v;}
    SLTerminalHandle term=0;
    int rc=slterminal_spawn(&term,command.c_str(),cols,rows);
    if(rc!=0){std::cerr<<"sleela native: cannot start '"<<cmdArgs[0]<<"': terminal error "<<rc<<"\n";reportMemoryManager();return 1;}
    char buf[4096];
    for(;;){
        SLTerminalCount n=slterminal_read(term,buf,sizeof buf);
        if(n<=0)break;
        std::cout.write(buf,(std::streamsize)n);
        std::cout.flush();
    }
    int closeRc=slterminal_close(term);
    reportMemoryManager();
    return closeRc==0?0:1;
}
static void lowerNativeModules(sleela::Program& prog){sleela::chemistry::lowerProgram(prog);sleela::financial::lowerProgram(prog);auto saved=prog.imports;
    // native::lowerProgram validates every remaining import against its own fixed
    // set of quantitative subject modules. chemistry/financial have their own
    // lowering above, and library imports (e.g. a versioned source line such as
    // `gui.version-military`, or any /lib package) are resolved by the library
    // pipeline -- not the native dispatcher. Hand native::lowerProgram only the
    // imports it actually owns so a valid library import is not misreported as an
    // "unknown native module", then restore the full import list.
    prog.imports.erase(std::remove_if(prog.imports.begin(),prog.imports.end(),[](const std::string&m){return !sleela::native::isModuleAvailable(m);}),prog.imports.end());
    sleela::native::lowerProgram(prog);prog.imports=std::move(saved);}
static int compileAndRun(sleela::Program& prog,const catalog::Catalog& cat,const sleela::SyntaxVersion& syntax={1,3}){
    SLVM*vm=slvm_new();if(!vm){std::cerr<<"sleelvac: unable to allocate Sleela VM\n";return 1;}
    slvm_set_audio_native_renderer(vm,sleela_audio_native_render_bridge,nullptr);
    int rc=0;
    try{
        lowerNativeModules(prog);
        sleela::compile(prog,vm,&cat,syntax);
        sleela::annotation::AnnotationRuntime annotations;
        std::string annotationError;
        if(!annotations.install(prog.annotations,annotationError)){
            std::cerr<<"sleelvac: annotation runtime rejected program: "<<annotationError<<"\n";
            slvm_free(vm);return 1;
        }
        if(annotations.hasForwarding()) std::cout<<"[annotation] Nexter Colony: "<<annotations.nexterColony()<<"\n";
        SLResult r=slvm_run(vm);
        if(r==SLR_ERROR){const char*e=slvm_error(vm);std::cerr<<"sleelvac: runtime error: "<<(e?e:"unknown")<<"\n";rc=1;}
    }catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";rc=1;}
    slvm_free(vm);return rc;
}
static catalog::Catalog loadCatalog(){
    // Try, in order: $SLEELA_SHEET; the catalog shipped under the installed
    // runtime root (so an installed binary finds SHEET.sheet from its install
    // location, not only from the checkout); the SLeeLa root resolved by
    // findRepoRoot; then cwd-relative fallbacks for working inside the checkout.
    std::vector<std::string> candidates;
    if(const char* env=std::getenv("SLEELA_SHEET"); env&&*env) candidates.emplace_back(env);
    if(fs::path installed=installRootFromExecutable(); !installed.empty())
        candidates.emplace_back((installed/"SHEET.sheet").string());
    if(fs::path repo=findRepoRoot(fs::current_path()); !repo.empty())
        candidates.emplace_back((repo/"SHEET.sheet").string());
    candidates.emplace_back("SHEET.sheet");
    candidates.emplace_back("../SHEET.sheet");
    candidates.emplace_back("../../SHEET.sheet");
    candidates.emplace_back("../../../SHEET.sheet");
    for(const auto& p:candidates){
        if(p.empty())continue;
        bool ok=false; catalog::Catalog c=catalog::parseCatalogFile(p.c_str(),&ok);
        if(ok)return c;
    }
    return catalog::Catalog{};
}
/* ---- `sleela vm` : select the virtual machine ---------------------------
 * The Native VM (impl/core, what every other subcommand runs on) is the
 * default. `sleela vm` lets a user list and select one of the versioned VMs
 * under virtual-machine/vm-version/NNN, whose Sleela manifests
 * (SLVmVersionNNN.sleela) + registry (SLVmRegistry.sleela) describe each build
 * and name the native main the host would load. This table mirrors those
 * manifests; selection is validated here the same way the OP_VM_SELECT service
 * opcode validates it in the VM (1..10). Actually building+booting a selected
 * version's native main is a host/build action beyond the shipped `sleela`
 * binary; this subcommand resolves and reports the selection. */
// Directory for the originated source document (syntax 2.4). Empty means the
// source file's own directory. Set by the compile subcommand's
// --originate-dir=<dir> flag.
static std::string g_originateDir;
// Wikitext emanation (syntax 2.4, third output). Opt-in via --wiki; the
// optional --wiki-dir=<dir> redirects the output (empty = source directory).
// g_wiki enables it; it is implied when --wiki-dir is given.
static bool        g_wiki=false;
static std::string g_wikiDir;
static int compileFile(const std::string&sourcePath,const std::string&outputPath);
static int runFile(const std::string&path);

static int vmCmd(int argc,char**argv){
    struct VmEntry{int n;const char*id;const char*label;const char*dir;const char*mainEntry;};
    static const VmEntry kVms[10]={
        {1,"idea-vm-001","ground floor","virtual-machine/vm-version/001","vm_ground_test"},
        {2,"idea-vm-002","memory hardening","virtual-machine/vm-version/002","vm_memory_test"},
        {3,"idea-vm-003","typed results","virtual-machine/vm-version/003","vm_result_test"},
        {4,"idea-vm-004","telemetry","virtual-machine/vm-version/004","vm_telemetry_test"},
        {5,"idea-vm-005","gc quality","virtual-machine/vm-version/005","vm_gc_test"},
        {6,"idea-vm-006","dispatch profiler","virtual-machine/vm-version/006","vm_profile_test"},
        {7,"idea-vm-007","capability enforcement","virtual-machine/vm-version/007","vm_capability_test"},
        {8,"idea-vm-008","platform parity","virtual-machine/vm-version/008","vm_platform_test"},
        {9,"idea-vm-009","audit trail","virtual-machine/vm-version/009","vm_audit_test"},
        {10,"idea-vm-010","consolidated","virtual-machine/vm-version/010","vm_runtime"},
    };
    /* Premium VMs: the eleven SLVM generations under sleela-virtual-machine.
     * Selected as `sleela vm select premium <1..11>` (or `pNN`). These are
     * substantial generation builds, distinct from the think-tank versions.
     * This table is mirrored by the SLeeLa-level premium catalogue
     * (sleela-virtual-machine/SLVmPremiumRegistry.sleela composing
     * SLVmPremiumVersion001..011), the premium sibling of the standard
     * virtual-machine/vm-version/SLVmRegistry; keep the two in step. */
    struct PremiumVm{int n;const char*id;const char*label;const char*dir;};
    static const PremiumVm kPremium[11]={
        {1,"slvm-1","dedicated VM: broker, capabilities, security, OS boundary","sleela-virtual-machine/001"},
        {2,"slvm-2","cryptographic and VM-link evolution","sleela-virtual-machine/002"},
        {3,"slvm-3","isolation and verification","sleela-virtual-machine/003"},
        {4,"slvm-4","authenticated/distributed execution","sleela-virtual-machine/004"},
        {5,"slvm-5","reproducibility, policy, recovery, assurance","sleela-virtual-machine/005"},
        {6,"slvm-6","continuous verification, lineage, attestation, leases, migration","sleela-virtual-machine/006"},
        {7,"slvm-7","hardened management, failure handling, recovery","sleela-virtual-machine/007"},
        {8,"slvm-8","supervised execution, admission, leases, transactions, audit","sleela-virtual-machine/008"},
        {9,"slvm-9","filesystem and OS adaptation","sleela-virtual-machine/009"},
        {10,"slvm-10","verified storage execution","sleela-virtual-machine/010"},
        {11,"slvm-11","filesystem-module hosting, schema-driven extension","sleela-virtual-machine/011"},
    };
    std::string sub = (argc>=3)? argv[2] : "list";
    if(sub=="list"||sub=="-h"||sub=="--help"){
        std::cout<<"Available virtual machines (the Native VM is the default):\n";
        std::cout<<"  native   Native VM (impl/core) — used unless you select another   [default]\n";
        std::cout<<"\nStandard (think-tank versions):\n";
        for(const auto&v:kVms)
            std::cout<<"  "<<v.n<<"  "<<v.id<<"  ["<<v.label<<"]  -> "<<v.dir<<"/"<<v.mainEntry<<"\n";
        std::cout<<"\nPremium (SLVM generations):\n";
        for(const auto&v:kPremium)
            std::cout<<"  premium "<<v.n<<"  "<<v.id<<"  ["<<v.label<<"]  -> "<<v.dir<<"\n";
        std::cout<<"\nUse: sleela vm select <native|1..10|premium 1..11>   |   sleela vm info <1..10|premium 1..11>\n";
        std::cout<<"     sleela vm run <premium <1..11>|native> <program.sleela>   (premium generations are real runnable VMs)\n";
        return 0;
    }
    // `sleela vm run premium <n> <program.sleela|artifact>` actually boots the
    // selected premium SLVM generation (a real VM linking the shared Core
    // engine) and runs the program on it. The generation is built on demand
    // (make -C its dir) if its binary is missing, the source is compiled to a
    // Core artifact via this same sleela binary, then the generation VM is
    // exec'd on that artifact. Standard (think-tank) versions are not yet
    // bootable this way; use the Native VM (plain `sleela run`).
    if(sub=="run"){
        // forms:  vm run premium <n> <prog>   |  vm run p<N> <prog>   |  vm run native <prog>
        if(argc<4){std::cerr<<"sleela vm run: expected <premium <1..11>|p<N>|native> <program>\n";return 2;}
        std::string sel=argv[3];
        std::string pnum; int progIdx=-1;
        if(sel=="premium"){ if(argc<6){std::cerr<<"sleela vm run: premium needs <n> <program>\n";return 2;} pnum=argv[4]; progIdx=5; }
        else if(sel.size()>=2 && (sel[0]=='p'||sel[0]=='P') && std::isdigit((unsigned char)sel[1])){ pnum=sel.substr(1); progIdx=4; }
        else if(sel=="native"){ if(argc<5){std::cerr<<"sleela vm run: native needs <program>\n";return 2;} std::cout<<"Native VM: running on impl/core (the default engine).\n"; return runFile(argv[4]); }
        else { std::cerr<<"sleela vm run: '"<<sel<<"' is not runnable here (use premium <1..11> or native)\n"; return 2; }
        if(progIdx>=argc){std::cerr<<"sleela vm run: missing <program>\n";return 2;}
        int n=std::atoi(pnum.c_str());
        if(n<1||n>11){std::cerr<<"sleela vm run: premium '"<<pnum<<"' out of range (1..11)\n";return 2;}
        const PremiumVm&v=kPremium[n-1];
        char nn[4]; std::snprintf(nn,sizeof(nn),"%02d",n);
        std::string vmbin=std::string(v.dir)+"/vm-build/slvm-"+nn;
        std::string prog=argv[progIdx];
        // Build the generation VM on demand if its binary is not present.
        if(!fs::exists(vmbin)){
            std::cout<<"Building premium VM "<<v.id<<" ("<<v.dir<<")...\n";
            if(runCommand("make -C "+shellQuote(v.dir))!=0 || !fs::exists(vmbin)){
                std::cerr<<"sleela vm run: could not build "<<vmbin
                         <<" (ensure the Core archive exists: make -C impl libslcore)\n";
                return 1;
            }
        }
        // Resolve the program to a Core artifact. If it is already a compiled
        // artifact, run it directly; otherwise compile it to a temp artifact.
        std::string artifact=prog; std::string tmp;
        if(!slvm_is_artifact_file(prog.c_str())){
            {
                auto uniq=std::chrono::steady_clock::now().time_since_epoch().count();
                tmp=(fs::temp_directory_path()/("slvm-run-"+std::to_string(uniq)+".sleela")).string();
            }
            if(compileFile(prog,tmp)!=0){std::cerr<<"sleela vm run: failed to compile "<<prog<<"\n";return 1;}
            artifact=tmp;
        }
        std::cout<<"Running "<<prog<<" on premium VM "<<v.id<<" ["<<v.label<<"].\n";
        int rc=runCommand(shellQuote(vmbin)+" "+shellQuote(artifact));
        if(!tmp.empty()){ std::error_code ec; fs::remove(tmp,ec); }
        return rc;
    }
    if(sub=="select"||sub=="info"){
        if(argc<4){std::cerr<<"sleela vm "<<sub<<": expected <native|1..10|premium 1..11>\n";return 2;}
        std::string sel=argv[3];
        // premium tier: `premium <n>` or `pNN`.
        bool premium=false; std::string pnum;
        if(sel=="premium"){ if(argc<5){std::cerr<<"sleela vm "<<sub<<": premium needs a number 1..11\n";return 2;} premium=true; pnum=argv[4]; }
        else if(sel.size()>=2 && (sel[0]=='p'||sel[0]=='P') && std::isdigit((unsigned char)sel[1])){ premium=true; pnum=sel.substr(1); }
        if(premium){
            int n=std::atoi(pnum.c_str());
            if(n<1||n>11){std::cerr<<"sleela vm "<<sub<<": premium '"<<pnum<<"' out of range (1..11)\n";return 2;}
            const PremiumVm&v=kPremium[n-1];
            if(sub=="info"){
                std::cout<<v.id<<"  [premium]  ["<<v.label<<"]\n"
                         <<"  generation dir : "<<v.dir<<"\n"
                         <<"  spec           : "<<v.dir<<"/LANGUAGE.SPEC.md\n"
                         <<"  build          : "<<v.dir<<"/Makefile\n";
                return 0;
            }
            std::cout<<"Selected premium VM "<<v.id<<" ["<<v.label<<"].\n"
                     <<"  Generation lives in "<<v.dir<<" (its own src/include/Makefile).\n"
                     <<"  This is a real runnable VM (it links the shared Core engine). Run a\n"
                     <<"  program on it with:  sleela vm run premium "<<n<<" <program.sleela>\n";
            return 0;
        }
        if(sel=="native"||sel=="0"){
            std::cout<<"Selected: Native VM (impl/core). This is the default; no load needed.\n";
            return 0;
        }
        int n=std::atoi(sel.c_str());
        if(n<1||n>10){std::cerr<<"sleela vm "<<sub<<": '"<<sel<<"' out of range (native | 1..10 | premium 1..11)\n";return 2;}
        const VmEntry&v=kVms[n-1];
        if(sub=="info"){
            std::cout<<v.id<<"  [standard]  ["<<v.label<<"]\n"
                     <<"  native dir : "<<v.dir<<"\n"
                     <<"  native main: "<<v.mainEntry<<"\n"
                     <<"  manifest   : "<<v.dir<<"/SLVmVersion"<<(n<10?"00":"0")<<n<<".sleela\n";
            return 0;
        }
        std::cout<<"Selected VM "<<v.id<<" ["<<v.label<<"].\n"
                 <<"  Its native main '"<<v.mainEntry<<"' lives in "<<v.dir<<".\n"
                 <<"  (OP_VM_SELECT "<<n<<" validated; building+booting a non-native VM's main\n"
                 <<"   is a host/build step beyond the shipped sleela binary.)\n";
        return 0;
    }
    std::cerr<<"sleela vm: unknown subcommand '"<<sub<<"' (use list | select | info)\n";
    return 2;
}
/* ---- SHA-256 (self-contained, for `sleela modium`) ----------------------
 * A compact, standard FIPS-180-4 SHA-256 over an in-memory buffer, so modium
 * can hash files without a new dependency. */
namespace sleela {
static std::string sha256_hex(const std::string& msg){
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
} // namespace sleela

/* ---- `sleela registry` : review/get/set the Sleela registry --------------
 * A terminal front end to the registry core (registry/sleela_registry.*): a
 * Windows-style key tree of settings + feature flags with sealed provenance.
 * A fresh install seeds a baseline; this command reviews or edits it. The
 * backing store is a portable .sleelareg file (default ./sleela.sleelareg or
 * $SLEELA_REGISTRY). */
static std::string registryStorePath(){
    const char* env=std::getenv("SLEELA_REGISTRY");
    if(env&&*env) return std::string(env);
    return "sleela.sleelareg";
}
static int registryCmd(int argc,char**argv){
    std::string store=registryStorePath();
    sl_registry* r=sl_reg_create();
    if(!r){std::cerr<<"sleela registry: out of memory\n";return 1;}
    /* load existing store, or seed a baseline if none exists */
    if(sl_reg_load(r,store.c_str())!=SL_REG_OK) sl_reg_seed_defaults(r);

    std::string sub=(argc>=3)?argv[2]:"review";
    int rc=0;
    if(sub=="review"||sub=="-h"||sub=="--help"){
        std::string path=(argc>=4)?argv[3]:"";
        std::string buf; buf.resize(1<<16);
        long n=sl_reg_review(r, path.empty()?nullptr:path.c_str(), &buf[0], buf.size());
        if(n>0){ buf.resize((size_t)n); std::cout<<buf; }
    } else if(sub=="get"){
        if(argc<5){std::cerr<<"usage: sleela registry get <key-path> <name>\n";rc=2;}
        else { char v[512]; long iv; int bv; sl_reg_kind k;
            if(sl_reg_value_kind(r,argv[3],argv[4],&k)!=SL_REG_OK){std::cerr<<"not found\n";rc=1;}
            else if(k==SL_REG_STRING&&sl_reg_get_string(r,argv[3],argv[4],v,sizeof v)==SL_REG_OK) std::cout<<v<<"\n";
            else if(k==SL_REG_INT&&sl_reg_get_int(r,argv[3],argv[4],&iv)==SL_REG_OK) std::cout<<iv<<"\n";
            else if(k==SL_REG_BOOL&&sl_reg_get_bool(r,argv[3],argv[4],&bv)==SL_REG_OK) std::cout<<(bv?"true":"false")<<"\n";
        }
    } else if(sub=="set"){
        if(argc<6){std::cerr<<"usage: sleela registry set <key-path> <name> <value> [--int|--bool]\n";rc=2;}
        else { bool asInt=false,asBool=false; for(int i=6;i<argc;++i){std::string a=argv[i]; if(a=="--int")asInt=true; else if(a=="--bool")asBool=true;}
            if(asInt) sl_reg_set_int(r,argv[3],argv[4],strtol(argv[5],nullptr,10));
            else if(asBool) sl_reg_set_bool(r,argv[3],argv[4], (std::string(argv[5])=="true"||std::string(argv[5])=="1")?1:0);
            else sl_reg_set_string(r,argv[3],argv[4],argv[5]);
            if(sl_reg_save(r,store.c_str())==SL_REG_OK) std::cout<<"set "<<argv[3]<<"/"<<argv[4]<<"\n"; else {std::cerr<<"save failed\n";rc=1;}
        }
    } else if(sub=="feature"){
        if(argc<4){std::cerr<<"usage: sleela registry feature <name> [on|off]\n";rc=2;}
        else if(argc>=5){ int on=(std::string(argv[4])=="on"||std::string(argv[4])=="1"); sl_reg_feature_set(r,argv[3],on); sl_reg_save(r,store.c_str()); std::cout<<"feature "<<argv[3]<<" = "<<(on?"on":"off")<<"\n"; }
        else std::cout<<"feature "<<argv[3]<<" = "<<(sl_reg_feature_enabled(r,argv[3])?"on":"off")<<"\n";
    } else if(sub=="save"){
        if(sl_reg_save(r,store.c_str())==SL_REG_OK) std::cout<<"saved "<<store<<"\n"; else {std::cerr<<"save failed\n";rc=1;}
    } else if(sub=="export"){
        const char* out=(argc>=4)?argv[3]:"registry.sql";
        if(sl_reg_export_sql(r,out)==SL_REG_OK) std::cout<<"exported MySQL script -> "<<out<<"\n"; else {std::cerr<<"export failed\n";rc=1;}
    } else { std::cerr<<"sleela registry: unknown subcommand '"<<sub<<"' (review|get|set|feature|save|export)\n"; rc=2; }
    sl_reg_destroy(r);
    return rc;
}

/* ---- `sleela modium` : scan a VM's files for resolution ------------------
 * For a selected VM version, verify every recorded file exists, sits where it
 * should, and matches its SHA-256 (the 95% hash scan) against that version's
 * MODIUM.json. `--exact` additionally re-reads bytes. Reconstitution of a
 * failed file comes from the version's rebuild.sql (a portable MySQL script
 * carrying each file's content); `restore` is EXPLICIT-ONLY and reports first.
 * Modium is a per-VM file-resolution scan the VM can run for itself — distinct
 * from the compiler's own build gate (verify-before-execution). */
static std::string modiumDirFor(int n){char b[64];std::snprintf(b,sizeof b,"virtual-machine/vm-version/%03d",n);return std::string(b);}
/* The author sealed into every modium entry (must match generate-modium.py). */
static const char* kModiumAuthor = "MEARVK LLC / the Author";
static int modiumScanOne(int n,bool exact,bool seal);
static int modiumScanOne(int n,bool exact){ return modiumScanOne(n,exact,false); }
static int modiumScanOne(int n,bool exact,bool seal){
    namespace fsx=std::filesystem;
    std::string dir=modiumDirFor(n);
    fsx::path manifest=fsx::path(dir)/"MODIUM.json";
    // search upward for the repo root holding the version dir (like the gate).
    fsx::path base=fsx::current_path(); fsx::path found;
    for(int up=0;up<8;++up){ if(fsx::exists(base/manifest)){found=base/manifest;break;} if(base.has_parent_path())base=base.parent_path(); else break; }
    if(found.empty()){std::cerr<<"sleela modium: MODIUM.json not found for idea-vm-"<<n<<" (run tools/generate-modium.py)\n";return 1;}
    fsx::path root=found.parent_path(); // the version folder
    std::string text; { std::ifstream in(found,std::ios::binary); std::stringstream ss; ss<<in.rdbuf(); text=ss.str(); }
    // minimal, dependency-free scan: pull each "path" and "sha256" pair in order.
    int total=0,okCount=0,missing=0,moved=0,bad=0,sealbad=0;
    size_t pos=0;
    while(true){ size_t pp=text.find("\"path\":",pos); if(pp==std::string::npos)break; size_t q1=text.find('"',pp+7); size_t q2=text.find('"',q1+1); std::string rel=text.substr(q1+1,q2-q1-1);
        /* bound this entry: fields belong to it until the next "path" (sorted
         * order per entry is path, seal, sha256, size). */
        size_t nextp=text.find("\"path\":",q2); if(nextp==std::string::npos) nextp=text.size();
        size_t sp=text.find("\"sha256\":",q2); if(sp==std::string::npos||sp>nextp){ pos=q2+1; total++; continue; }
        size_t s1=text.find('"',sp+9); size_t s2=text.find('"',s1+1); std::string want=text.substr(s1+1,s2-s1-1); pos=q2+1; total++;
        fsx::path fp=root/rel;
        if(!fsx::exists(fp)){missing++;std::cerr<<"  MISSING  "<<rel<<"\n";continue;}
        // SHA-256 of the file
        std::ifstream f(fp,std::ios::binary); if(!f){moved++;std::cerr<<"  UNREADABLE "<<rel<<"\n";continue;}
        std::stringstream fss; fss<<f.rdbuf(); std::string body=fss.str();
        std::string got=sleela::sha256_hex(body); // helper below
        if(got!=want){bad++;std::cerr<<"  CORRUPT  "<<rel<<" (sha mismatch"<<(exact?", exact":"")<<")\n";continue;}
        if(seal){
            // originality seal: SHA-256(vm | path | sha256 | author). Recompute
            // from the file's ACTUAL hash + its recorded path, and compare to
            // the recorded seal. Catches a record whose entry was re-sealed under
            // a different identity/author, not just file-vs-hash drift.
            // pull the recorded seal for this entry (seal sits between this
            // entry's "path" and the next entry's "path", per sorted order).
            size_t slp=text.find("\"seal\":",q2); std::string wantSeal;
            if(slp!=std::string::npos && slp<nextp){ size_t a=text.find('"',slp+7); size_t b2=text.find('"',a+1); wantSeal=text.substr(a+1,b2-a-1); }
            char vmtag[16]; std::snprintf(vmtag,sizeof vmtag,"idea-vm-%03d",n);
            std::string sealInput=std::string(vmtag)+"|"+rel+"|"+got+"|"+kModiumAuthor;
            std::string gotSeal=sleela::sha256_hex(sealInput);
            if(gotSeal!=wantSeal){ sealbad++; std::cerr<<"  UNSEALED "<<rel<<" (originality seal mismatch)\n"; continue; }
        }
        okCount++;
    }
    std::cout<<"idea-vm-"<<n<<": "<<okCount<<"/"<<total<<" ok"
             <<(missing?(" missing="+std::to_string(missing)):"")
             <<(bad?(" corrupt="+std::to_string(bad)):"")
             <<(sealbad?(" unsealed="+std::to_string(sealbad)):"")
             <<(moved?(" unreadable="+std::to_string(moved)):"")
             <<(seal?"  [seal]":(exact?"  [exact]":"  [sha256]"))<<"\n";
    return (missing||bad||moved||sealbad)?1:0;
}
static int modiumCmd(int argc,char**argv){
    std::string sub=(argc>=3)?argv[2]:"status";
    bool exact=false,sealMode=false; for(int i=2;i<argc;++i){std::string a=argv[i]; if(a=="--exact")exact=true; else if(a=="--seal")sealMode=true;}
    if(sub=="-h"||sub=="--help"){
        std::cout<<"sleela modium — scan a VM's files for resolution (exist, placed, intact)\n"
                   "  sleela modium scan [native|1..10] [--exact]   verify files vs MODIUM.json\n"
                   "  sleela modium status                          summary across all versions\n"
                   "  sleela modium restore <1..10> [--from-sql]    reconstitute from rebuild.sql (explicit; reports first)\n";
        return 0;
    }
    if(sub=="scan"){
        std::string target=(argc>=4 && argv[3][0]!='-')?argv[3]:"";
        if(target=="native"){std::cout<<"Native VM (impl/core) is covered by the build SHA-256 gate, not modium.\n";return 0;}
        if(target.empty()){int rc=0;for(int n=1;n<=10;++n)rc|=modiumScanOne(n,exact,sealMode);return rc;}
        int n=std::atoi(target.c_str()); if(n<1||n>10){std::cerr<<"sleela modium scan: '"<<target<<"' out of range (native or 1..10)\n";return 2;}
        return modiumScanOne(n,exact,sealMode);
    }
    if(sub=="status"){int rc=0;for(int n=1;n<=10;++n)rc|=modiumScanOne(n,false);return rc;}
    if(sub=="restore"){
        if(argc<4){std::cerr<<"sleela modium restore: expected <1..10>\n";return 2;}
        int n=std::atoi(argv[3]); if(n<1||n>10){std::cerr<<"out of range\n";return 2;}
        std::cout<<"modium restore (idea-vm-"<<n<<"): reporting mismatches first (restore is explicit-only).\n";
        int rc=modiumScanOne(n,false);
        if(rc==0){std::cout<<"  nothing to restore — all files resolve.\n";return 0;}
        std::cout<<"  to reconstitute, load this version's rebuild.sql into MySQL and write back\n"
                   "  the recorded content for the files flagged above (or restore from git).\n"
                   "  (modium never overwrites files automatically.)\n";
        return 0;
    }
    std::cerr<<"sleela modium: unknown subcommand '"<<sub<<"' (use scan | status | restore)\n";
    return 2;
}
static int designActivityCmd(int argc,char**argv){
    if(argc<9){std::cerr<<"Usage: sleela design-activity <science> <correctness> <reproducibility> <observability> <safety> <resource> <interoperability>\n";return 2;}
    std::string science=argv[2]; if(science.rfind("--science=",0)==0)science=science.substr(10);
    SLDA_ScienceDomain domain;
    if(slda_parse_domain(science.c_str(),&domain)!=0){std::cerr<<"sleela: unknown science domain '"<<science<<"'\n";return 2;}
    double raw[SLDA_DIMENSIONS];
    for(int i=0;i<SLDA_DIMENSIONS;i++){char*end=nullptr;raw[i]=std::strtod(argv[i+3],&end);if(!end||*end||!std::isfinite(raw[i])||raw[i]<0||raw[i]>100){std::cerr<<"sleela: design activity score "<<(i+1)<<" must be 0..100\n";return 2;}}
    SLDA_Vector normalized,expected; SLDA_Result result{};
    if(slda_normalize(raw,&normalized)!=0||slda_science_profile(domain,&expected)!=0||slda_compare(&normalized,&expected,&result)!=0){std::cerr<<"sleela: design activity calculation failed\n";return 1;}
    result.domain=domain;
    char json[1024];slda_format_json("sleela",&result,json,sizeof json);
    std::cout<<json<<"\n"; return 0;
}
static std::string cliProgramName = "sleela";
// Usage for the `sleelvac` compiler command. Sleelvac shares the native driver
// with `sleela`, but it is the SLeeLa *compiler*: its job is to turn source into
// a runnable Core artifact and to check/validate that artifact. This text
// describes only that compiler surface, so `sleelvac --help` is truly about
// sleelvac and not a verbatim copy of the full `sleela` front-end listing.
static int sleelvacUsage(){
    const std::string &n = cliProgramName;
    std::cerr
        << "Sleelvac - the SLeeLa compiler"
        << " (syntax spec " << sleela::minSupportedSyntax().str()
        << " - " << sleela::maxSupportedSyntax().str() << ")\n"
        << "\n"
        << "Compiles SLeeLa source into runnable Core artifacts and checks them.\n"
        << "(For running programs, servers, the VM, the registry and JVM-family\n"
        << " input, use the `sleela` front end.)\n"
        << "\n"
        << "Global options (before the command):\n"
        << "  --memory-manager, --mm       Enable the Memory Manager with the default 2 GiB limit\n"
        << "  --memory-manager=SIZE        Enable it with a byte limit; K, M, and G suffixes are accepted\n"
        << "  --mm=SIZE                    Short form of --memory-manager=SIZE\n"
        << "\n"
        << "Compiler commands:\n"
        << "  " << n << " [global-options] compile <file.sleela> [more.sleela ...] -o <output.sleela>\n"
        << "                               Compile one or more Sleela source files to a runnable Core artifact\n"
        << "                               (author-marked documents also emit an originated source document;\n"
        << "                                add --wiki or --wiki-dir=<dir> to also emanate a MediaWiki page)\n"
        << "  " << n << " check <file.sleela>\n"
        << "                               Parse, resolve referenced library units, and check semantics (no output artifact)\n"
        << "  " << n << " validate-artifact <file.sleela>\n"
        << "                               Validate a compiled artifact's Core ABI and format\n"
        << "  " << n << " version\n"
        << "                               Print the Sleelvac compiler version string\n"
        << "\n"
        << "Notes:\n"
        << "  - `compile` writes a persistent <output.sleela> Core artifact plus a sibling\n"
        << "    <output.sleela>.annotations metadata file.\n"
        << "  - Native math/physics/economics/chemistry/financial module calls are lowered at compile time.\n"
        << "  - Execution is gated by SHA-256 verification; the trusted manifest must verify before a build runs.\n"
        << "  - The global memory limit may also be set by the SLeeLa configuration/environment.\n";
    return 2;
}
static int usage(){
    const std::string &n = cliProgramName;
    // When invoked as `sleelvac`, present the compiler's own surface rather than
    // the full `sleela` front end. Sleelvac is the SLeeLa *compiler*: it turns
    // source into runnable Core artifacts and checks/validates them. The run,
    // server, VM, registry and framework-input commands belong to the `sleela`
    // front end; sleelvac only documents what it is actually for.
    if(fs::path(cliProgramName).stem().string()=="sleelvac") return sleelvacUsage();
    std::cerr
        << "Sleela - SLeeLa compiler and runtime front end"
        << " (syntax spec " << sleela::minSupportedSyntax().str()
        << " - " << sleela::maxSupportedSyntax().str() << ")\n"
        << "\n"
        << "Global options (before the command):\n"
        << "  --memory-manager, --mm       Enable the Memory Manager with the default 2 GiB limit\n"
        << "  --memory-manager=SIZE        Enable it with a byte limit; K, M, and G suffixes are accepted\n"
        << "  --mm=SIZE                    Short form of --memory-manager=SIZE\n"
        << "\n"
        << "Source and artifact commands:\n"
        << "  " << n << " [global-options] compile <file.sleela> [more.sleela ...] -o <output.sleela>\n"
        << "                               Compile one or more Sleela source files to a runnable Core artifact\n"
        << "  " << n << " check <file.sleela>\n"
        << "                               Parse, resolve referenced library units, and check semantics\n"
        << "  " << n << " run <file.sleela|file.xclass|file.java|file.jar> [arguments ...]\n"
        << "  " << n << " run --class-path <path> <MainClass> [arguments ...]\n"
        << "  " << n << " run --class <MainClass> [arguments ...]\n"
        << "                               Run SLeeLa input, Java source, runnable JAR, or compiled Java class\n"
        << "  " << n << " validate-artifact <file.sleela>\n"
        << "                               Validate the runnable artifact ABI and format\n"
        << "  " << n << " version\n"
        << "                               Print the compiler/runtime version string\n"
        << "\n"
        << "Other commands:\n"
        << "  " << n << " http-server <1|2|3> [--port N] [--threads N] [--root DIR] [--log FILE] [--once]\n"
        << "  " << n << " xclass [--run|--emit|--info] <file.xclass> [more-files ...]\n"
        << "  " << n << " langin [--run|--emit-sleela|--emit-xclass|--info] <file.java|file.kt|file.scala|file.groovy|file.clj> [more-files ...]\n"
        << "  " << n << " nordshrift [--emit|--roundtrip] [--target=sleela|java|c] <file.sleela>\n"
        << "  " << n << " vm [list|select <native|1..10|premium 1..11>|info <selection>|run <premium <1..11>|p<N>|native> <program>]\n"
        << "  " << n << " modium [scan [native|1..10] [--exact] [--seal]|status|restore <1..10>]\n"
        << "  " << n << " registry [review [key-path]|get <key-path> <name>|set <key-path> <name> <value> [--int|--bool]|feature <name> [on|off]|save|export [file]]\n"
        << "  " << n << " native [--config <file>|-c <file>] [--memory-manager[=<size>]|--mm[=<size>]] [--] <program> [args...]\n"
        << "  " << n << " exec [--config <file>|-c <file>] [--memory-manager[=<size>]|--mm[=<size>]] [--] <program> [args...]\n"
        << "  " << n << " design-activity <science> <correctness> <reproducibility> <observability> <safety> <resource> <interoperability>\n"
        << "  " << n << " defender <detect|fetch|build|install|provision> [directory] [--allow-defender|--optin] [--allow-root] [--sha256 <hex>|--sha256=<hex>]\n"
        << "\n"
        << "Command-specific option details:\n"
        << "  xclass: --run (default), --emit, --info\n"
        << "  langin: --run (default), --emit-sleela (alias --emit), --emit-xclass, --info\n"
        << "  nordshrift: --emit, --roundtrip, --target=sleela (default)|java|c\n"
        << "  native/exec: --config FILE (alias -c; also --config=FILE); --memory-manager[=SIZE] or --mm[=SIZE]; -- ends option parsing\n"
        << "  defender: detect is read-only; fetch/build/install/provision require --allow-defender or SLEELA_DEFENDER_OPTIN=1;\n"
        << "           --sha256 HEX or --sha256=HEX (or SLEELA_DEFENDER_SHA256) supplies the expected archive hash;\n"
        << "           --allow-root (or SLEELA_DEFENDER_ALLOW_ROOT=1) explicitly authorizes privileged installation\n"
        << "  Global memory limit may also be set by the SLeeLa configuration/environment.\n";
    return 2;
}
static bool checkSyntaxVersion(const std::string& path,const std::string& src){sleela::VersionResolution v=sleela::resolveSyntaxVersion(src);if(v.isError()){std::cerr<<"sleelvac: "<<path<<": error: "<<v.message<<"\n";return false;}if(v.isWarning())std::cerr<<"sleelvac: "<<path<<": warning: "<<v.message<<"\n";
    // Creator-document schema binding: if the source declares a #schema pragma,
    // the referenced XSD must exist and its version must be understood. Absent
    // is fine (not every source is a Creator document).
    std::string dir=fs::path(path).parent_path().string();
    sleela::SchemaResolution s=sleela::resolveSchemaReference(src,dir);
    if(s.isError()){std::cerr<<"sleelvac: "<<path<<": error: "<<s.message<<"\n";return false;}
    if(s.present())std::cerr<<"sleelvac: "<<path<<": "<<s.message<<"\n";
    return true;}
static bool parseSource(const std::string&path,std::string&src,sleela::Program&prog,sleela::VersionResolution&version){if(!readFile(path,src)){std::cerr<<"sleelvac: cannot open '"<<path<<"'\n";return false;}if(!checkSyntaxVersion(path,src))return false;version=sleela::resolveSyntaxVersion(src);try{sleela::Lexer lexer(src);auto tokens=lexer.tokenize();sleela::Parser parser(std::move(tokens));prog=parser.parseProgram();sleela::Program validation;validation.imports=prog.imports;validation.imports.erase(std::remove_if(validation.imports.begin(),validation.imports.end(),[](const std::string& module){return !sleela::native::isModuleAvailable(module);}),validation.imports.end());sleela::native::validateImports(validation);return true;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<path<<": "<<ex.what()<<"\n";return false;}}
static bool loadReferencedLibraryUnits(const std::string& source, sleela::Program& program);
static int checkFile(const std::string&path){
    if(verifyBeforeExecution(fs::current_path()))return 1;
    std::string src;sleela::Program prog;sleela::VersionResolution v;
    if(!parseSource(path,src,prog,v))return 1;
    if(!loadReferencedLibraryUnits(src,prog))return 1;
    try{
        sleela::SemanticResult sem=sleela::analyzeSemantics(prog,v.declared);
        for(const auto&w:sem.warnings) std::cerr<<"sleelvac: "<<path<<": warning: "<<w<<"\n";
        if(!sem.ok()){std::cerr<<"sleelvac: "<<path<<": error: "<<sem.errors.front()<<"\n";return 1;}
        sleela::chemistry::lowerProgram(prog);sleela::financial::lowerProgram(prog);
    }catch(const std::exception&ex){std::cerr<<"sleelvac: "<<path<<": "<<ex.what()<<"\n";return 1;}
    std::cout<<path<<": ok (syntax "<<(v.pragmaPresent?"declared ":"assumed ")<<v.declared.str()<<")\n";return 0;
}
static int compileFile(const std::string&sourcePath,const std::string&outputPath){if(verifyBeforeExecution(fs::current_path()))return 1;std::string src;sleela::Program prog;sleela::VersionResolution syntax;if(!parseSource(sourcePath,src,prog,syntax))return 1;if(!loadReferencedLibraryUnits(src,prog))return 1;catalog::Catalog cat=loadCatalog();try{int rc=sleela::compileToArtifact(prog,outputPath,&cat,syntax.declared);if(rc!=0)return 1;std::cout<<"sleelvac: "<<sourcePath<<" -> "<<outputPath<<" (runnable Sleela Core artifact)\n";
    // Author-marked documents (syntax 2.4) get a second, originated source
    // document alongside the artifact: every member located, originated, and
    // branded with a therefore-brander. Clean and united.
    if(prog.authorMarked){std::string op=sleela::writeOriginatedDocument(prog,sourcePath,g_originateDir);if(!op.empty())std::cout<<"sleelvac: originated source -> "<<op<<"\n";}
    // Wikitext emanation (syntax 2.4, third output): a consequence of compiling
    // an author-marked document, opt-in via --wiki/--wiki-dir. Factual,
    // structured MediaWiki markup for a self-hosted wiki.
    if(prog.authorMarked&&g_wiki){std::string wp=sleela::writeWikitextDocument(prog,sourcePath,g_wikiDir);if(!wp.empty())std::cout<<"sleelvac: wikitext emanated -> "<<wp<<"\n";}
    return 0;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static int validateArtifact(const std::string&path){
    char error[256]={0};
    if(!slvm_validate_artifact_file(path.c_str(),error,sizeof(error))){
        std::cerr<<"sleelvac: artifact ABI validation failed: "<<(error[0]?error:"unknown error")<<"\n";
        return 1;
    }
    std::cout<<"artifact ABI ok: "<<path<<" (runtime "<<SLEELA_VM_ABI_MAJOR<<"."<<SLEELA_VM_ABI_MINOR<<", format "<<SLEELA_ARTIFACT_FORMAT_VERSION<<")\n";
    return 0;
}
static int runArtifact(const std::string&path){
    SLVM*vm=slvm_load_file(path.c_str());
    if(!vm){std::cerr<<"sleelvac: cannot load runnable .sleela artifact '"<<path<<"'\n";return 1;}
    slvm_set_audio_native_renderer(vm,sleela_audio_native_render_bridge,nullptr);
    sleela::annotation::DocumentAnnotations data;
    std::ifstream af(path+".annotations");std::string line;
    while(std::getline(af,line)){auto eq=line.find('=');if(eq!=std::string::npos&&eq>0)data.add(line.substr(0,eq),line.substr(eq+1));}
    sleela::annotation::AnnotationRuntime annotations;std::string annotationError;
    if(!annotations.install(data,annotationError)){std::cerr<<"sleelvac: artifact annotation runtime rejected: "<<annotationError<<"\n";slvm_free(vm);return 1;}
    if(annotations.hasForwarding())std::cout<<"[annotation] Nexter Colony: "<<annotations.nexterColony()<<"\n";
    SLResult r=slvm_run(vm);
    if(r==SLR_ERROR){const char*e=slvm_error(vm);std::cerr<<"sleelvac: runtime error: "<<(e?e:"unknown")<<"\n";slvm_free(vm);return 1;}
    slvm_free(vm);return 0;
}
// Launch a child process with an argv vector, without routing Java arguments
// through a shell or the interactive PTY. This preserves whitespace/metacharacters
// in application arguments and returns the child's actual exit status.
static int runExternalProcess(const std::vector<std::string>& args){
    if(args.empty()||args[0].empty()){
        std::cerr<<"sleela: cannot launch an empty process command.\n";
        return 2;
    }
#if defined(_WIN32)
    std::vector<std::wstring> wide;
    wide.reserve(args.size());
    for(const auto& arg:args){
        int count=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,arg.c_str(),-1,nullptr,0);
        if(count<=0){
            std::cerr<<"sleela: process argument is not valid UTF-8.\n";
            return 2;
        }
        std::wstring value((size_t)count,L'\0');
        if(MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,arg.c_str(),-1,&value[0],count)<=0){
            std::cerr<<"sleela: unable to convert process argument to UTF-16.\n";
            return 2;
        }
        value.resize((size_t)count-1);
        wide.push_back(std::move(value));
    }
    std::vector<const wchar_t*> argv;
    argv.reserve(wide.size()+1);
    for(const auto& arg:wide)argv.push_back(arg.c_str());
    argv.push_back(nullptr);
    intptr_t status=_wspawnvp(_P_WAIT,wide[0].c_str(),argv.data());
    if(status==-1){
        std::cerr<<"sleela: unable to start Java process (errno "<<errno<<").\n";
        return errno==ENOENT?127:126;
    }
    return (int)status;
#else
    std::vector<char*> argv;
    argv.reserve(args.size()+1);
    for(const auto& arg:args)argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);
    pid_t child=fork();
    if(child<0){
        std::cerr<<"sleela: fork failed while launching Java: "<<std::strerror(errno)<<"\n";
        return 127;
    }
    if(child==0){
        execvp(argv[0],argv.data());
        std::perror("sleela: execvp");
        _exit(errno==ENOENT?127:126);
    }
    int status=0;
    while(waitpid(child,&status,0)<0){
        if(errno==EINTR)continue;
        std::cerr<<"sleela: waitpid failed while collecting Java exit status: "<<std::strerror(errno)<<"\n";
        return 127;
    }
    if(WIFEXITED(status))return WEXITSTATUS(status);
    if(WIFSIGNALED(status))return 128+WTERMSIG(status);
    return 1;
#endif
}
static int runJavaFrameworkSource(const std::string&path,const std::vector<std::string>&programArgs={}){
    std::string src;
    if(!readFile(path,src)){
        std::cerr<<"sleelvac: cannot open Java source '"<<path<<"'\n";
        return 1;
    }
    SleelaJavaProgramRequest request{};
    request.classpath=".";
    request.source_file=path.c_str();
    SleelaJavaRuntimeProbeResult probe{};
    SleelaJavaProgramPlan plan{};
    int dispatch=sleela_java_runtime_dispatch_source(src.data(),src.size(),path.c_str(),
                                                      &request,&probe,&plan);
    if(dispatch<0){
        std::cerr<<"sleelvac: Java runtime dispatch failed for '"<<path<<"'\n";
        return 1;
    }
    const char* javaMode=std::getenv("SLEELA_JAVA_MODE");
    const char* javaEnabled=std::getenv("SLEELA_JAVA_ENABLED");
    if((javaMode&&std::string(javaMode)=="disabled") ||
       (javaEnabled&&std::string(javaEnabled)=="0")){
        std::cerr<<"[java] Java execution is disabled by SLEELA_JAVA_MODE or SLEELA_JAVA_ENABLED.\n";
        return 2;
    }
    if(dispatch==1 || probe.action==SLEELA_JAVA_ACTION_PROMPT_INSTALL){
        std::cerr<<"[java] Java VM required for "<<path<<" but no local VM was found.\n"
                 <<"       Set SLEELA_JAVA or JAVA_HOME, or install/configure a Java distribution, then retry.\n";
        return 2;
    }
    /* A .java source file is Java even when it uses no GUI framework.
       Execute it with the discovered JVM instead of treating it as a SLeeLa
       source-ingestion unit. */
    if(!plan.ready || !probe.java_executable || !probe.java_executable[0]){
        std::cerr<<"sleelvac: Java provider did not produce a runnable JVM handoff for '"<<path<<"'\n";
        return 1;
    }

    /* Build a real Java process argument vector. JavaFX module options must
       precede the source-file operand; never infer module availability merely
       from finding the java executable. */
    std::vector<std::string> args={probe.java_executable};
    if(probe.kind==SLEELA_JAVA_FX){
        const char* modulePath=std::getenv("SLEELA_JAVAFX_MODULE_PATH");
        const char* configuredModules=std::getenv("SLEELA_JAVAFX_MODULES");
        if(modulePath&&*modulePath){
            args.push_back("--module-path");
            args.emplace_back(modulePath);
            args.push_back("--add-modules");
            args.emplace_back((configuredModules&&*configuredModules)
                ? configuredModules : "javafx.controls,javafx.graphics");
        }else{
            std::cerr<<"[javafx] no module path configured; launch will work only if "
                        "JavaFX is already available to this Java runtime. Set "
                        "SLEELA_JAVAFX_MODULE_PATH and optionally SLEELA_JAVAFX_MODULES.\\n";
        }
#if !defined(_WIN32) && !defined(__APPLE__)
        const char* display=std::getenv("DISPLAY");
        const char* wayland=std::getenv("WAYLAND_DISPLAY");
        if((!display||!*display)&&(!wayland||!*wayland))
            std::cerr<<"[javafx] no DISPLAY or WAYLAND_DISPLAY detected; graphical startup may fail on this headless host.\n";
#endif
    }
    args.push_back(path);
    for(const auto&arg:programArgs) args.push_back(arg);
    std::cout<<"[java] secondary circuit: "<<sleela_java_runtime_kind_name(probe.kind)
             <<" -> "<<probe.java_executable<<" -> "<<path<<"\n";
    std::cout.flush();
    return runExternalProcess(args);
}
static int runJavaApplication(const std::vector<std::string>&javaArgs,const std::string&description){
    const char* javaMode=std::getenv("SLEELA_JAVA_MODE");
    const char* javaEnabled=std::getenv("SLEELA_JAVA_ENABLED");
    if((javaMode&&std::string(javaMode)=="disabled") ||
       (javaEnabled&&std::string(javaEnabled)=="0")){
        std::cerr<<"[java] Java execution is disabled by SLEELA_JAVA_MODE or SLEELA_JAVA_ENABLED.\n";
        return 2;
    }
    SleelaJavaRuntimeProbeResult probe{};
    if(sleela_java_runtime_probe_local_vm(&probe)!=1 ||
       !probe.java_vm_available || !probe.java_executable[0]){
        std::cerr<<"[java] A Java runtime is required for "<<description<<" but was not found.\n"
                 <<"       Set SLEELA_JAVA or JAVA_HOME, or install/configure a Java distribution, then retry.\n";
        return 2;
    }
    std::vector<std::string> args={probe.java_executable};
    args.insert(args.end(),javaArgs.begin(),javaArgs.end());
    std::cout<<"[java] external JVM: "<<probe.java_executable<<" -> "<<description<<"\n";
    std::cout.flush();
    return runExternalProcess(args);
}
static bool validJavaMainClass(const std::string&name){
    // Accept conventional binary class names, including nested classes ($).
    // A class name is a single argument, never a shell fragment or JVM option.
    static const std::regex pattern(
        R"(^[A-Za-z_$][A-Za-z0-9_$]*(\.[A-Za-z_$][A-Za-z0-9_$]*)*$)");
    return std::regex_match(name,pattern);
}
static int runJavaJar(const std::string&jar,const std::vector<std::string>&programArgs){
    std::error_code ec;
    if(!fs::is_regular_file(fs::path(jar),ec)){
        std::cerr<<"sleela: Java JAR not found or not a regular file: "<<jar<<"\n";
        return 2;
    }
    std::vector<std::string> args={"-jar",jar};
    args.insert(args.end(),programArgs.begin(),programArgs.end());
    return runJavaApplication(args,"JAR "+jar);
}
static int runJavaClass(const std::string&classpath,const std::string&mainClass,
                        const std::vector<std::string>&programArgs){
    if(!validJavaMainClass(mainClass)){
        std::cerr<<"sleela: invalid Java main class name '"<<mainClass<<"'.\n"
                 <<"       Use a binary class name such as com.example.Main.\n";
        return 2;
    }
    std::vector<std::string> args={"-cp",classpath.empty()?".":classpath,mainClass};
    args.insert(args.end(),programArgs.begin(),programArgs.end());
    return runJavaApplication(args,"class "+mainClass+" (classpath "+(classpath.empty()?".":classpath)+")");
}
static int runSource(const std::string&path){std::string src;sleela::Program prog;sleela::VersionResolution syntax;if(!parseSource(path,src,prog,syntax))return 1;if(!loadReferencedLibraryUnits(src,prog))return 1;catalog::Catalog cat=loadCatalog();return compileAndRun(prog,cat,syntax.declared);}
// Combine several parsed Programs into one compilation unit by concatenating
// their classes/structs/imports (and merging document annotations). This lets a
// user split a program across multiple .sleela files passed at run/compile time;
// downstream stages already treat the whole Program as one unit and will report
// any duplicate struct/class/method across files.
static void mergeProgram(sleela::Program& into,sleela::Program&& from);
static bool loadReferencedLibraryUnits(const std::string& initialSource,sleela::Program& program){
    // Source libraries are compiled into the same Program/artifact as the client.
    // This deliberately resolves only class-like unit names referenced by source,
    // rather than blindly parsing every file under /lib (which can introduce
    // unrelated duplicate declarations and incompatible syntax editions).
    std::string libRoot=resolveLibraryRoot();
    sleela::library::Index index=sleela::library::Index::discover(libRoot);
    if(index.empty()) return true; // standalone installations may intentionally have no source library
    std::set<std::string> loadedPaths;
    std::string source=initialSource;
    for(const auto& c:program.classes) loadedPaths.insert(c.name);
    for(const auto& s:program.structs) loadedPaths.insert(s.name);
    std::regex identifier(R"(\b[A-Za-z_][A-Za-z0-9_]*\b)");
    // Ignore comments and string/character literals during dependency discovery.
    // Documentation text must not trigger unrelated library loads (for example,
    // the word "Certificate" in a comment must not resolve Java API classes).
    auto dependencyScanText = [](const std::string& input) {
        std::string output = input;
        enum class State { Code, LineComment, BlockComment, AngleBlockComment, String, Character };
        State state = State::Code;
        bool escaped = false;
        for (size_t i = 0; i < input.size(); ++i) {
            const char c = input[i];
            const char next = (i + 1 < input.size()) ? input[i + 1] : '\0';
            if (state == State::Code) {
                bool linePrefix = true;
                for (size_t p = i; p > 0 && input[p - 1] != '\n'; --p) {
                    const char before = input[p - 1];
                    if (before != ' ' && before != '\t' && before != '\r') { linePrefix = false; break; }
                }
                if (linePrefix && c == '<' && next == '<') {
                    output[i] = output[i + 1] = ' '; ++i; state = State::AngleBlockComment;
                } else if (linePrefix && c == '<' && next != '<') {
                    size_t close = i + 1;
                    while (close < input.size() && input[close] != '\n' && input[close] != '>') ++close;
                    if (close < input.size() && input[close] == '>') { output[i] = ' '; state = State::LineComment; }
                } else if (c == '/' && next == '/') {
                    output[i] = output[i + 1] = ' ';
                    ++i;
                    state = State::LineComment;
                } else if (c == '/' && next == '*') {
                    output[i] = output[i + 1] = ' ';
                    ++i;
                    state = State::BlockComment;
                } else if (c == '"') {
                    output[i] = ' ';
                    state = State::String;
                    escaped = false;
                } else if (c == '\'') {
                    output[i] = ' ';
                    state = State::Character;
                    escaped = false;
                }
            } else if (state == State::LineComment) {
                if (c == '\n') state = State::Code;
                else output[i] = ' ';
            } else if (state == State::BlockComment) {
                if (c == '*' && next == '/') { output[i] = output[i + 1] = ' '; ++i; state = State::Code; }
                else if (c != '\n') output[i] = ' ';
            } else if (state == State::AngleBlockComment) {
                if (c == '>' && next == '>') { output[i] = output[i + 1] = ' '; ++i; state = State::Code; }
                else if (c != '\n') output[i] = ' ';
            } else {
                if (c != '\n') output[i] = ' ';
                if (escaped) escaped = false;
                else if (c == '\\') escaped = true;
                else if ((state == State::String && c == '"') ||
                         (state == State::Character && c == '\'')) state = State::Code;
            }
        }
        return output;
    };
    // Resolve in passes so a loaded unit can pull in its own referenced units.
    for(size_t pass=0;pass<64;++pass){
        std::set<std::string> names;
        const std::string scanSource = dependencyScanText(source);
        for(std::sregex_iterator it(scanSource.begin(),scanSource.end(),identifier),end;it!=end;++it){
            const std::string name=it->str();
            if(!name.empty() && std::isupper(static_cast<unsigned char>(name[0]))) names.insert(name);
        }
        bool changed=false;
        for(const auto& name:names){
            if(loadedPaths.count(name)) continue;
            const sleela::library::Symbol* match=nullptr;
            // A versioned library import such as `import gui.version-military;`
            // selects that source subtree for duplicate class names.
            std::vector<std::string> preferredRoots;
            for(const auto& imported : program.imports){
                std::string root=imported;
                std::replace(root.begin(),root.end(),'.','/');
                if(root.find('/')!=std::string::npos) preferredRoots.push_back(root);
            }
            auto underPreferredRoot=[&](const sleela::library::Symbol& symbol){
                for(const auto& root:preferredRoots)
                    if(symbol.path.rfind(root + "/",0)==0) return true;
                return false;
            };
            bool hasPreferredMatch=false;
            for(const auto& symbol:index.symbols())
                if(symbol.name==name && underPreferredRoot(symbol)) hasPreferredMatch=true;
            // Generated JDK API documentation contains .sleela envelopes whose
            // class names duplicate the actual library classes (for example,
            // java/early_access/jdk28/docs/api/.../String.sleela vs
            // java/java/lang/String.sleela). Prefer a real source unit whenever
            // one exists; only consider docs/api envelopes as a last resort.
            bool hasNonDocumentationMatch=false;
            for(const auto& symbol:index.symbols()){
                if(symbol.name!=name) continue;
                if(hasPreferredMatch && !underPreferredRoot(symbol)) continue;
                if(symbol.path.find("/docs/api/")==std::string::npos)
                    hasNonDocumentationMatch=true;
            }
            for(const auto& symbol:index.symbols()){
                if(symbol.name!=name) continue;
                if(hasPreferredMatch && !underPreferredRoot(symbol)) continue;
                if(hasNonDocumentationMatch &&
                   symbol.path.find("/docs/api/")!=std::string::npos) continue;
                if(match && match->path!=symbol.path){
                    std::cerr<<"sleelvac: ambiguous library symbol '"<<name<<"' in /lib ("
                             <<match->path<<" and "<<symbol.path<<"); add a versioned import such as 'import gui.version-military;' to select the intended library line.\n";
                    return false;
                }
                match=&symbol;
            }
            if(!match) continue;
            const std::string path=(fs::path(index.root())/match->path).string();
            std::string unitSource;
            if(!readFile(path,unitSource)){
                std::cerr<<"sleelvac: cannot read library unit '"<<path<<"'\n";
                return false;
            }
            try{
                sleela::Lexer lexer(unitSource);
                sleela::Parser parser(lexer.tokenize());
                sleela::Program unit=parser.parseProgram();
                // Mark the path before merging to prevent dependency cycles.
                loadedPaths.insert(name);
                for(const auto& cls:unit.classes) loadedPaths.insert(cls.name);
                for(const auto& st:unit.structs) loadedPaths.insert(st.name);
                source += "\n" + unitSource;
                mergeProgram(program,std::move(unit));
                changed=true;
            }catch(const std::exception& ex){
                std::cerr<<"sleelvac: library unit '"<<path<<"': "<<ex.what()<<"\n";
                return false;
            }
        }
        if(!changed) return true;
    }
    std::cerr<<"sleelvac: library dependency expansion exceeded 64 passes; possible dependency cycle.\n";
    return false;
}

static void mergeProgram(sleela::Program& into,sleela::Program&& from){
    for(auto& s:from.structs) into.structs.push_back(std::move(s));
    for(auto& c:from.classes) into.classes.push_back(std::move(c));
    for(auto& i:from.imports) into.imports.push_back(std::move(i));
    for(auto& d:from.dynamiteImports) into.dynamiteImports.push_back(std::move(d));
    for(auto& p:from.permissibleImports) into.permissibleImports.push_back(std::move(p));
    for(const auto& a:from.annotations.all()) into.annotations.add(a);
}
// Run one or more .sleela files as a single merged program. Each file is parsed
// independently; the first file's resolved syntax version governs the unit.
static int runSourceMulti(const std::vector<std::string>& paths){
    if(paths.empty())return 1;
    sleela::Program merged; sleela::VersionResolution firstSyntax; bool haveSyntax=false;
    for(const auto& path:paths){
        std::string src; sleela::Program prog; sleela::VersionResolution syntax;
        if(!parseSource(path,src,prog,syntax))return 1;
        if(!loadReferencedLibraryUnits(src,prog))return 1;
        if(!haveSyntax){firstSyntax=syntax;haveSyntax=true;}
        mergeProgram(merged,std::move(prog));
    }
    catalog::Catalog cat=loadCatalog();
    return compileAndRun(merged,cat,firstSyntax.declared);
}
// Compile one or more .sleela files into a single runnable artifact.
static int compileFilesMulti(const std::vector<std::string>& paths,const std::string& outputPath){
    if(verifyBeforeExecution(fs::current_path()))return 1;
    if(paths.empty())return 1;
    sleela::Program merged; sleela::VersionResolution firstSyntax; bool haveSyntax=false;
    for(const auto& path:paths){
        std::string src; sleela::Program prog; sleela::VersionResolution syntax;
        if(!parseSource(path,src,prog,syntax))return 1;
        if(!loadReferencedLibraryUnits(src,prog))return 1;
        if(!haveSyntax){firstSyntax=syntax;haveSyntax=true;}
        mergeProgram(merged,std::move(prog));
    }
    catalog::Catalog cat=loadCatalog();
    try{
        int rc=sleela::compileToArtifact(merged,outputPath,&cat,firstSyntax.declared);
        if(rc!=0)return 1;
        std::cout<<"sleelvac: "<<paths.size()<<" source file(s) -> "<<outputPath<<" (runnable Sleela Core artifact)\n";
        return 0;
    }catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}
}
static int runXclass(const std::vector<std::string>&paths){catalog::Catalog cat=loadCatalog();try{sleela::xclass::Loaded loaded=sleela::xclass::loadFiles(paths);std::cout<<"[xclass] ingested "<<loaded.metas.size()<<" SecureJDK 28 class(es):\n";for(const auto&m:loaded.metas)std::cout<<"[xclass]   "<<sleela::xclass::infoLine(m)<<"\n";return compileAndRun(loaded.program,cat);}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static int xclassCmd(int argc,char**argv){enum{RUN,EMIT,INFO}mode=RUN;std::vector<std::string>files;for(int i=2;i<argc;i++){std::string a=argv[i];if(a=="--emit")mode=EMIT;else if(a=="--info")mode=INFO;else if(a=="--run")mode=RUN;else if(a.rfind("--",0)==0){std::cerr<<"sleelvac: unknown option "<<a<<"\n";return 2;}else files.push_back(a);}if(files.empty()){std::cerr<<"sleela xclass: no .xclass files given\n";return 2;}if(verifyBeforeExecution(fs::current_path()))return 1;if(mode==RUN)return runXclass(files);try{sleela::xclass::Loaded loaded=sleela::xclass::loadFiles(files);if(mode==EMIT)std::cout<<loaded.emitted;else for(const auto&m:loaded.metas){std::cout<<sleela::xclass::infoLine(m)<<"\n";if(!m.sourceFile.empty())std::cout<<"  source    : "<<m.sourceFile<<"\n";if(!m.edition.empty())std::cout<<"  edition   : "<<m.edition<<"\n";if(!m.signatureHex.empty())std::cout<<"  signature : "<<m.signatureAlg<<":"<<m.signatureHex<<(m.signed_?" (signed)":"")<<"\n";}return 0;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static bool readFile(const std::string&path,std::string&out){std::ifstream f(path,std::ios::binary);if(!f)return false;std::ostringstream ss;ss<<f.rdbuf();out=ss.str();return true;}
// True if `path` is a JVM-family source input (Java/Kotlin/Scala/Groovy/Clojure).
static bool isLangInput(const std::string&path){return sleela::langin::languageForPath(path)!=sleela::langin::Language::Unknown;}
// Ingest JVM-family source files and run the reconstructed Program.
static int runLangin(const std::vector<std::string>&paths){catalog::Catalog cat=loadCatalog();try{sleela::langin::Loaded loaded=sleela::langin::loadFiles(paths);std::cout<<"[langin] ingested "<<loaded.metas.size()<<" JVM-family unit(s):\n";for(const auto&m:loaded.metas)std::cout<<"[langin]   "<<sleela::langin::infoLine(m)<<"\n";return compileAndRun(loaded.program,cat);}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
// `sleela langin [--run|--emit-sleela|--emit-xclass|--info] <file...>`
static int langinCmd(int argc,char**argv){enum{RUN,EMIT_SLEELA,EMIT_XCLASS,INFO}mode=RUN;std::vector<std::string>files;for(int i=2;i<argc;i++){std::string a=argv[i];if(a=="--run")mode=RUN;else if(a=="--emit-sleela"||a=="--emit")mode=EMIT_SLEELA;else if(a=="--emit-xclass")mode=EMIT_XCLASS;else if(a=="--info")mode=INFO;else if(a.rfind("--",0)==0){std::cerr<<"sleelvac: unknown option "<<a<<"\n";return 2;}else files.push_back(a);}if(files.empty()){std::cerr<<"sleela langin: no source files given (.java/.kt/.scala/.groovy/.clj)\n";return 2;}if(verifyBeforeExecution(fs::current_path()))return 1;if(mode==RUN)return runLangin(files);try{sleela::langin::Loaded loaded=sleela::langin::loadFiles(files);if(mode==EMIT_SLEELA)std::cout<<loaded.emitted;else if(mode==EMIT_XCLASS)std::cout<<sleela::langin::emitXclass(loaded.program,loaded.metas);else for(const auto&m:loaded.metas)std::cout<<sleela::langin::infoLine(m)<<"\n";return 0;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static int runFile(const std::string&path){
    if(slvm_is_artifact_file(path.c_str()))return runArtifact(path);
    if(hasExt(path,".xclass"))return runXclass({path});
    if(hasExt(path,".java")){
        std::string src;
        if(readFile(path,src)){
            SleelaJavaRuntimeProbeResult probe{};
            SleelaJavaProgramRequest request{};
            request.classpath=".";
            request.source_file=path.c_str();
            SleelaJavaProgramPlan plan{};
            int dispatch=sleela_java_runtime_dispatch_source(src.data(),src.size(),path.c_str(),
                                                              &request,&probe,&plan);
            if(dispatch<0)return 1;
            return runJavaFrameworkSource(path);
        }
    }
    if(isLangInput(path))return runLangin({path});
    return runSource(path);
}
// `sleela nordshrift [--emit] [--target=sleela|java|c] <file.sleela>`  (SLeeLa -> Nordshrift)
// `sleela nordshrift --roundtrip <file.sleela>`  (SLeeLa -> Nordshrift(Sleela) -> re-parse -> run + verify)
static int nordshriftCmd(int argc,char**argv){
    enum{EMIT,ROUNDTRIP}mode=EMIT;nordshrift::TargetLang target=nordshrift::TargetLang::Sleela;std::string pkg;std::string file;
    for(int i=2;i<argc;i++){std::string a=argv[i];
        if(a=="--emit")mode=EMIT;
        else if(a=="--roundtrip")mode=ROUNDTRIP;
        else if(a=="--target=sleela")target=nordshrift::TargetLang::Sleela;
        else if(a=="--target=java")target=nordshrift::TargetLang::Java;
        else if(a=="--target=c")target=nordshrift::TargetLang::C;
        else if(a.rfind("--package=",0)==0)pkg=a.substr(10);
        else if(a.rfind("--",0)==0){std::cerr<<"sleelvac: unknown option "<<a<<"\n";return 2;}
        else file=a;
    }
    if(file.empty()){std::cerr<<"sleela nordshrift: need a <file.sleela>\n";return 2;}
    if(verifyBeforeExecution(fs::current_path()))return 1;
    // SLeeLa -> Nordshrift: parse the Sleela source and transpile via the emitter.
    std::string src;sleela::Program prog;sleela::VersionResolution ver;
    if(!parseSource(file,src,prog,ver))return 1;
    std::string nordshriftForm;
    try{nordshriftForm=nordshrift::emitProgram(prog,target,pkg);}catch(const std::exception&ex){std::cerr<<"sleelvac: nordshrift emit failed: "<<ex.what()<<"\n";return 1;}
    if(mode==EMIT){std::cout<<nordshriftForm;return 0;}
    // --roundtrip: force the Sleela target, re-parse it, and run it back on the core.
    std::string sleelaForm;
    try{sleelaForm=nordshrift::emitProgram(prog,nordshrift::TargetLang::Sleela,"");}catch(const std::exception&ex){std::cerr<<"sleelvac: nordshrift emit failed: "<<ex.what()<<"\n";return 1;}
    sleela::Program back;
    try{sleela::Lexer lx(sleelaForm);auto tk=lx.tokenize();sleela::Parser ps(std::move(tk));back=ps.parseProgram();}
    catch(const std::exception&ex){std::cerr<<"sleelvac: nordshrift round-trip re-parse failed: "<<ex.what()<<"\n";return 1;}
    // Verify structural fidelity. The meaningful check is that the program's
    // shape survives the round trip: same classes, and per class the same field
    // and method counts. (Exact text equality is a stronger, secondary check --
    // the emitter fully parenthesizes/normalizes, so a second pass can differ
    // cosmetically even when the program is identical.)
    auto shapeOf=[](const sleela::Program&p){std::string s;s+="C"+std::to_string(p.classes.size());for(const auto&c:p.classes){s+="|"+c.name+":f"+std::to_string(c.fields.size())+":m"+std::to_string(c.methods.size());}return s;};
    bool structurallyEqual=(shapeOf(prog)==shapeOf(back));
    std::string reEmit;
    try{reEmit=nordshrift::emitProgram(back,nordshrift::TargetLang::Sleela,"");}catch(const std::exception&){reEmit="";}
    bool textFixed=(!reEmit.empty()&&reEmit==sleelaForm);
    std::cerr<<"[nordshrift] SLeeLa -> Nordshrift(Sleela) -> re-parse: "
             <<(structurallyEqual?"structure preserved":"WARNING: structure changed")
             <<(textFixed?" (exact fixed point)":" (canonical form stable; re-emit normalizes)")<<"\n";
    if(!structurallyEqual)return 1;
    catalog::Catalog cat=loadCatalog();
    return compileAndRun(back,cat,ver.declared);
}
int main(int argc,char**argv){
    if(argc>0 && argv[0] && *argv[0]) cliProgramName = fs::path(argv[0]).filename().string();
    if(argc<2)return usage();
    bool mmEnabled=true; size_t mmLimit=kDefaultMemoryLimit; std::string memoryConfig;
    if(const char*cfg=std::getenv("SLEELA_CONFIG_FILE"))memoryConfig=cfg; else if(fs::exists("sleela.conf"))memoryConfig="sleela.conf";
    if(!memoryConfig.empty()){bool ce=mmEnabled;if(!loadMemoryConfig(memoryConfig,mmLimit,ce))return 2;mmEnabled=ce;}
    {size_t l=0;if(memoryManagerRequested(l)){mmEnabled=true;mmLimit=l;}}
    int start=1; while(start<argc){std::string a=argv[start];
      if(a=="--memory-manager"||a=="--mm"){mmEnabled=true;mmLimit=kDefaultMemoryLimit;++start;}
      else if(a.rfind("--memory-manager=",0)==0||a.rfind("--mm=",0)==0){size_t l=0;if(!parseByteSize(a.substr(a.find('=')+1),l)){std::cerr<<"sleela: invalid memory-manager size\n";return 2;}mmEnabled=true;mmLimit=l;++start;}
      else break;
    }
    if(mmEnabled&&!enableMemoryManager(mmLimit,true))return 2;
    if(start>=argc)return usage();
    std::string cmd=argv[start];
    if(start>1){int n=argc-start;std::vector<char*> av;av.reserve((size_t)n+1);for(int i=start;i<argc;++i)av.push_back(argv[i]);av.push_back(nullptr);argc=n;argv=av.data();}
    if(cmd=="http-server")return sleelaHttpServerCommand(argc,argv);
    if(cmd=="native"||cmd=="exec")return nativeCmd(argc,argv);
    int rc=0;
    if(cmd=="compile"){
        // compile <file...> -o <out>: collect every source path before "-o".
        std::vector<std::string> files; std::string out;
        for(int i=2;i<argc;++i){std::string a=argv[i];
            if(a=="-o"){if(i+1<argc){out=argv[i+1];i++;}else return usage();}
            else if(a.rfind("--originate-dir=",0)==0){g_originateDir=a.substr(a.find('=')+1);}
            else if(a=="--wiki"){g_wiki=true;}
            else if(a.rfind("--wiki-dir=",0)==0){g_wiki=true;g_wikiDir=a.substr(a.find('=')+1);}
            else files.push_back(a);}
        if(files.empty()||out.empty())return usage();
        rc=(files.size()==1)?compileFile(files[0],out):compileFilesMulti(files,out);
    }
    else if(cmd=="check"){if(argc<3)return usage();rc=checkFile(argv[2]);}
    else if(cmd=="validate-artifact"){if(argc!=3)return usage();rc=validateArtifact(argv[2]);}
    else if(cmd=="run"){if(argc<3)return usage();if(verifyBeforeExecution(fs::current_path()))return 1;if(hasExt(argv[2],".jar")){std::vector<std::string> javaArgs;for(int i=3;i<argc;++i)javaArgs.emplace_back(argv[i]);rc=runJavaJar(argv[2],javaArgs);}else if(std::string(argv[2])=="--class-path"||std::string(argv[2])=="--classpath"||std::string(argv[2])=="-cp"){if(argc<5){std::cerr<<"sleela run: expected --class-path <path> <MainClass> [arguments...]\n";return 2;}std::vector<std::string> javaArgs;for(int i=5;i<argc;++i)javaArgs.emplace_back(argv[i]);rc=runJavaClass(argv[3],argv[4],javaArgs);}else if(std::string(argv[2])=="--class"){if(argc<4){std::cerr<<"sleela run: expected --class <MainClass> [arguments...]\n";return 2;}std::vector<std::string> javaArgs;for(int i=4;i<argc;++i)javaArgs.emplace_back(argv[i]);rc=runJavaClass(".",argv[3],javaArgs);}else if(hasExt(argv[2],".xclass")){std::vector<std::string>files;for(int i=2;i<argc;++i)files.push_back(argv[i]);rc=runXclass(files);}else if(isLangInput(argv[2])){std::vector<std::string>files;for(int i=2;i<argc;++i)files.push_back(argv[i]);if(hasExt(argv[2],".java")){std::string src;if(readFile(argv[2],src)){SleelaJavaRuntimeProbeResult probe{};SleelaJavaProgramRequest request{};request.classpath=".";request.source_file=argv[2];SleelaJavaProgramPlan plan{};int dispatch=sleela_java_runtime_dispatch_source(src.data(),src.size(),argv[2],&request,&probe,&plan);if(dispatch<0)return 1;{std::vector<std::string> javaArgs;for(int i=3;i<argc;++i)javaArgs.emplace_back(argv[i]);rc=runJavaFrameworkSource(argv[2],javaArgs);}}else rc=runLangin(files);}else rc=runLangin(files);}else if(argc>3){std::vector<std::string>files;for(int i=2;i<argc;++i)files.push_back(argv[i]);rc=runSourceMulti(files);}else rc=runFile(argv[2]);}
    else if(cmd=="xclass"){if(argc<3)return usage();rc=xclassCmd(argc,argv);}
    else if(cmd=="langin"){if(argc<3)return usage();rc=langinCmd(argc,argv);}
    else if(cmd=="nordshrift"){if(argc<3)return usage();rc=nordshriftCmd(argc,argv);}
    else if(cmd=="vm")return vmCmd(argc,argv);
    else if(cmd=="modium")return modiumCmd(argc,argv);
    else if(cmd=="registry")return registryCmd(argc,argv);
    else if(cmd=="design-activity")return designActivityCmd(argc,argv);
    else if(cmd=="defender")return defenderCmd(argc,argv);
    else if(cmd=="version"){
        // The identity is baked in at compile time: the `sleela` binary always
        // reports the Language/Runtime; the `sleelvac` binary always reports the
        // compiler. This does NOT depend on argv[0], so a launcher, symlink, or
        // alias can never make `sleela version` surface the Sleelvac banner.
        // Explicit flags still allow forcing either banner from either binary:
        //     version --runtime    (force the Language/Runtime banner)
        //     version --compiler   (force the Sleelvac compiler banner)
        bool compiler = kBuiltAsCompiler;
        for(int i=2;i<argc;i++){
            const std::string a=argv[i];
            if(a=="--runtime"||a=="--sleela")      compiler=false;
            else if(a=="--compiler"||a=="--sleelvac") compiler=true;
        }
        std::cout<<(compiler?sleelvacVersionBanner():sleelaVersionBanner())<<"\n";
        return 0;
    }
    else if(hasExt(cmd,".sleela")||hasExt(cmd,".xclass")||isLangInput(cmd)){if(verifyBeforeExecution(fs::current_path()))return 1;rc=runFile(cmd);}
    else return usage();
    if(mmEnabled){reportMemoryManager();}
    return rc;
}
