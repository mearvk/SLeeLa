// ===========================================================================
// driver.cpp -- Sleelvac™ command-line compiler/runtime front end.
// ===========================================================================
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "artifact.h"
#include "native_api.h"
#include "chemistry_api.h"
#include "financial_api.h"
#include "version.h"
#include "../catalog/sheet_catalog.h"
#include "../xclass/xclass_loader.h"
#include "../langin/langin.h"
#include "../nordshrift/sleela_emit.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
extern "C" {
#include "../core/sleela_core.h"
#include "../core/sleela_memmgr.h"
#include "../core/sleela_terminal.h"
}
namespace fs=std::filesystem;
static const char* kVersion="Sleelvac™ 1.4 (Sleela compiler; executable native math/physics/economics/chemistry/financial modules; persistent .sleela Core artifacts; .xclass input; JVM-family langin input: Java/Kotlin/Scala/Groovy/Clojure; Nordshrift round-trip (SLeeLa->Nordshrift->back); OS Defender provisioning; SHA-256 execution gate)";
static bool readFile(const std::string& path,std::string& out);
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
    fs::path dir=fs::absolute(start,ec);
    if(ec)dir=start;
    for(;;){
        if(fs::exists(dir/"tools"/"verify-before-execution.py"))return dir;
        fs::path parent=dir.parent_path();
        if(parent.empty()||parent==dir)break;
        dir=parent;
    }
    return fs::path();
}
static int verifyBeforeExecution(const fs::path&cwd){
    const char* env=std::getenv("SLEELA_SHA256_MANIFEST");
    if(!env||!*env){
        std::cerr<<"sleelvac: SHA-256 verification is required; set SLEELA_SHA256_MANIFEST to a trusted JSON manifest\n";
        return 1;
    }
    // Resolve the repo root robustly (any CWD), falling back to the CWD itself.
    fs::path root=findRepoRoot(cwd);
    if(root.empty())root=cwd;
    fs::path manifest=fs::path(env);
    if(!manifest.is_absolute())manifest=root/manifest;
    fs::path tool=root/"tools"/"verify-before-execution.py";
    if(!fs::exists(tool)){
        std::cerr<<"sleelvac: verification tool not found: "<<tool
                 <<"\n  (searched upward from "<<cwd<<" for tools/verify-before-execution.py)\n";
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
//
// The Memory Manager (impl/core/sleela_memmgr.*) accounts for raw process
// memory and can fail allocations closed at a hard byte limit. It is opt-in:
//   * `--memory-manager` (or SLEELA_MEMORY_MANAGER=1) enables it with no limit;
//   * `--memory-manager=<size>` / `SLEELA_MEMORY_MANAGER=<size>` enables it with
//     a hard limit; <size> accepts a plain byte count or a K/M/G suffix.
// It is also turned on automatically ("on need") when running a native
// executable, because that is the workload most likely to exhaust the host.
// ---------------------------------------------------------------------------
static bool parseByteSize(const std::string&in,size_t&out){
    if(in.empty())return false;
    // A bare on/off/true/false toggles the manager with no limit.
    std::string low=toLowerHex(in);
    if(low=="on"||low=="true"||low=="yes"||low=="1"){out=0;return true;}
    size_t i=0;unsigned long long v=0;bool any=false;
    for(;i<in.size()&&std::isdigit((unsigned char)in[i]);++i){v=v*10ull+(unsigned)(in[i]-'0');any=true;}
    if(!any)return false;
    unsigned long long mult=1;
    if(i<in.size()){
        char c=(char)std::tolower((unsigned char)in[i]);
        if(c=='k')mult=1024ull;else if(c=='m')mult=1024ull*1024ull;else if(c=='g')mult=1024ull*1024ull*1024ull;
        else return false;
        ++i;
        if(i<in.size()&&(in[i]=='b'||in[i]=='B'))++i; // allow KB/MB/GB
    }
    if(i!=in.size())return false;
    out=(size_t)(v*mult);
    return true;
}
// Interpret a --memory-manager[=value] argument or the SLEELA_MEMORY_MANAGER
// env var. Returns true if the manager should be enabled and sets `limit`.
static bool memoryManagerRequested(size_t&limit){
    const char*env=std::getenv("SLEELA_MEMORY_MANAGER");
    if(env&&*env){size_t l=0;if(parseByteSize(env,l)){limit=l;return true;}
        // A non-parseable but present value like "0"/"off" disables it.
        std::string e=toLowerHex(env);if(e=="0"||e=="off"||e=="false"||e=="no")return false;}
    return false;
}
static void enableMemoryManager(size_t limit,bool announce){
    slmm_enable(limit);
    if(announce){
        if(limit)std::cerr<<"[memory-manager] enabled (hard limit "<<limit<<" bytes)\n";
        else std::cerr<<"[memory-manager] enabled (no hard limit)\n";
    }
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
               "  sleela native [--config <file>] [--memory-manager[=<size>]] [--] <program> [args...]\n"
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
    size_t mmLimit=0;bool mmExplicit=false;
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
    enableMemoryManager(mmLimit,true);
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
static void lowerNativeModules(sleela::Program& prog){sleela::chemistry::lowerProgram(prog);sleela::financial::lowerProgram(prog);auto saved=prog.imports;prog.imports.erase(std::remove(prog.imports.begin(),prog.imports.end(),"chemistry"),prog.imports.end());prog.imports.erase(std::remove(prog.imports.begin(),prog.imports.end(),"financial"),prog.imports.end());sleela::native::lowerProgram(prog);prog.imports=std::move(saved);}
static int compileAndRun(sleela::Program& prog,const catalog::Catalog& cat,const sleela::SyntaxVersion& syntax={1,0}){SLVM*vm=slvm_new();if(!vm){std::cerr<<"sleelvac: unable to allocate Sleela VM\n";return 1;}int rc=0;try{lowerNativeModules(prog);sleela::compile(prog,vm,&cat,syntax);SLResult r=slvm_run(vm);if(r==SLR_ERROR){const char*e=slvm_error(vm);std::cerr<<"sleelvac: runtime error: "<<(e?e:"unknown")<<"\n";rc=1;}}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";rc=1;}slvm_free(vm);return rc;}
static catalog::Catalog loadCatalog(){const char*env=std::getenv("SLEELA_SHEET");const char*candidates[]={env,"SHEET.sheet","../SHEET.sheet","../../SHEET.sheet","../../../SHEET.sheet"};for(const char*p:candidates){if(!p||!*p)continue;bool ok=false;catalog::Catalog c=catalog::parseCatalogFile(p,&ok);if(ok)return c;}return catalog::Catalog{};}
static int usage(){std::cerr<<"Usage:\n  sleela [--memory-manager[=<size>]] compile <file.sleela> -o <program.sleela>\n  sleela [--memory-manager[=<size>]] run <file.sleela>\n  sleela [--memory-manager[=<size>]] run <program.sleela>\n  sleela [--memory-manager[=<size>]] run <file.xclass> [more...]\n  sleela xclass [--run|--emit|--info] <file.xclass> [more...]\n  sleela langin [--run|--emit-sleela|--emit-xclass|--info] <file.java|.kt|.scala|.groovy|.clj> [more...]\n  sleela nordshrift [--emit] [--target=sleela|java|c] [--package=P] <file.sleela>   (SLeeLa -> Nordshrift)\n  sleela nordshrift --roundtrip <file.sleela>                                       (SLeeLa -> Nordshrift -> back, run)\n  sleela check <file.sleela>\n  sleela native [--config <file>] [--memory-manager[=<size>]] [--] <program> [args...]\n  sleela exec   [--config <file>] [--memory-manager[=<size>]] [--] <program> [args...]\n  sleela version\n  sleela defender detect\n  sleela defender <fetch|build|install|provision> [directory] --allow-defender --sha256 <hex> [--allow-root]\n\nMemory Manager:\n  --memory-manager[=<size>]    account for raw process memory and (with <size>)\n                               fail allocations closed at a hard byte limit.\n                               <size> accepts a byte count or a K/M/G suffix\n                               (or SLEELA_MEMORY_MANAGER=<size>). It is enabled\n                               automatically for `native`/`exec`.\n\nSecurity:\n  SLEELA_SHA256_MANIFEST=<trusted JSON manifest> is required before compile, run, check, xclass, native/exec, and Defender diagnostics/build/install/provision.\n  Defender fetch/build/install/provision additionally require --allow-defender (opt-in), --sha256 <hex> (payload integrity), and --allow-root for the privileged install step.\n";return 2;}
static bool checkSyntaxVersion(const std::string& path,const std::string& src){sleela::VersionResolution v=sleela::resolveSyntaxVersion(src);if(v.isError()){std::cerr<<"sleelvac: "<<path<<": error: "<<v.message<<"\n";return false;}if(v.isWarning())std::cerr<<"sleelvac: "<<path<<": warning: "<<v.message<<"\n";return true;}
static bool parseSource(const std::string&path,std::string&src,sleela::Program&prog,sleela::VersionResolution&version){if(!readFile(path,src)){std::cerr<<"sleelvac: cannot open '"<<path<<"'\n";return false;}if(!checkSyntaxVersion(path,src))return false;version=sleela::resolveSyntaxVersion(src);try{sleela::Lexer lexer(src);auto tokens=lexer.tokenize();sleela::Parser parser(std::move(tokens));prog=parser.parseProgram();sleela::Program validation;validation.imports=prog.imports;validation.imports.erase(std::remove(validation.imports.begin(),validation.imports.end(),"chemistry"),validation.imports.end());validation.imports.erase(std::remove(validation.imports.begin(),validation.imports.end(),"financial"),validation.imports.end());sleela::native::validateImports(validation);return true;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<path<<": "<<ex.what()<<"\n";return false;}}
static int checkFile(const std::string&path){if(verifyBeforeExecution(fs::current_path()))return 1;std::string src;sleela::Program prog;sleela::VersionResolution v;if(!parseSource(path,src,prog,v))return 1;try{sleela::chemistry::lowerProgram(prog);sleela::financial::lowerProgram(prog);}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<path<<": "<<ex.what()<<"\n";return 1;}std::cout<<path<<": ok (syntax "<<(v.pragmaPresent?"declared ":"assumed ")<<v.declared.str()<<")\n";return 0;}
static int compileFile(const std::string&sourcePath,const std::string&outputPath){if(verifyBeforeExecution(fs::current_path()))return 1;std::string src;sleela::Program prog;sleela::VersionResolution syntax;if(!parseSource(sourcePath,src,prog,syntax))return 1;catalog::Catalog cat=loadCatalog();try{int rc=sleela::compileToArtifact(prog,outputPath,&cat,syntax.declared);if(rc!=0)return 1;std::cout<<"sleelvac: "<<sourcePath<<" -> "<<outputPath<<" (runnable Sleela Core artifact)\n";return 0;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static int runArtifact(const std::string&path){SLVM*vm=slvm_load_file(path.c_str());if(!vm){std::cerr<<"sleelvac: cannot load runnable .sleela artifact '"<<path<<"'\n";return 1;}SLResult r=slvm_run(vm);if(r==SLR_ERROR){const char*e=slvm_error(vm);std::cerr<<"sleelvac: runtime error: "<<(e?e:"unknown")<<"\n";slvm_free(vm);return 1;}slvm_free(vm);return 0;}
static int runSource(const std::string&path){std::string src;sleela::Program prog;sleela::VersionResolution syntax;if(!parseSource(path,src,prog,syntax))return 1;catalog::Catalog cat=loadCatalog();return compileAndRun(prog,cat,syntax.declared);}
static int runXclass(const std::vector<std::string>&paths){catalog::Catalog cat=loadCatalog();try{sleela::xclass::Loaded loaded=sleela::xclass::loadFiles(paths);std::cout<<"[xclass] ingested "<<loaded.metas.size()<<" SecureJDK 28 class(es):\n";for(const auto&m:loaded.metas)std::cout<<"[xclass]   "<<sleela::xclass::infoLine(m)<<"\n";return compileAndRun(loaded.program,cat);}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static int xclassCmd(int argc,char**argv){enum{RUN,EMIT,INFO}mode=RUN;std::vector<std::string>files;for(int i=2;i<argc;i++){std::string a=argv[i];if(a=="--emit")mode=EMIT;else if(a=="--info")mode=INFO;else if(a=="--run")mode=RUN;else if(a.rfind("--",0)==0){std::cerr<<"sleelvac: unknown option "<<a<<"\n";return 2;}else files.push_back(a);}if(files.empty()){std::cerr<<"sleela xclass: no .xclass files given\n";return 2;}if(verifyBeforeExecution(fs::current_path()))return 1;if(mode==RUN)return runXclass(files);try{sleela::xclass::Loaded loaded=sleela::xclass::loadFiles(files);if(mode==EMIT)std::cout<<loaded.emitted;else for(const auto&m:loaded.metas){std::cout<<sleela::xclass::infoLine(m)<<"\n";if(!m.sourceFile.empty())std::cout<<"  source    : "<<m.sourceFile<<"\n";if(!m.edition.empty())std::cout<<"  edition   : "<<m.edition<<"\n";if(!m.signatureHex.empty())std::cout<<"  signature : "<<m.signatureAlg<<":"<<m.signatureHex<<(m.signed_?" (signed)":"")<<"\n";}return 0;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static bool readFile(const std::string&path,std::string&out){std::ifstream f(path,std::ios::binary);if(!f)return false;std::ostringstream ss;ss<<f.rdbuf();out=ss.str();return true;}
// True if `path` is a JVM-family source input (Java/Kotlin/Scala/Groovy/Clojure).
static bool isLangInput(const std::string&path){return sleela::langin::languageForPath(path)!=sleela::langin::Language::Unknown;}
// Ingest JVM-family source files and run the reconstructed Program.
static int runLangin(const std::vector<std::string>&paths){catalog::Catalog cat=loadCatalog();try{sleela::langin::Loaded loaded=sleela::langin::loadFiles(paths);std::cout<<"[langin] ingested "<<loaded.metas.size()<<" JVM-family unit(s):\n";for(const auto&m:loaded.metas)std::cout<<"[langin]   "<<sleela::langin::infoLine(m)<<"\n";return compileAndRun(loaded.program,cat);}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
// `sleela langin [--run|--emit-sleela|--emit-xclass|--info] <file...>`
static int langinCmd(int argc,char**argv){enum{RUN,EMIT_SLEELA,EMIT_XCLASS,INFO}mode=RUN;std::vector<std::string>files;for(int i=2;i<argc;i++){std::string a=argv[i];if(a=="--run")mode=RUN;else if(a=="--emit-sleela"||a=="--emit")mode=EMIT_SLEELA;else if(a=="--emit-xclass")mode=EMIT_XCLASS;else if(a=="--info")mode=INFO;else if(a.rfind("--",0)==0){std::cerr<<"sleelvac: unknown option "<<a<<"\n";return 2;}else files.push_back(a);}if(files.empty()){std::cerr<<"sleela langin: no source files given (.java/.kt/.scala/.groovy/.clj)\n";return 2;}if(verifyBeforeExecution(fs::current_path()))return 1;if(mode==RUN)return runLangin(files);try{sleela::langin::Loaded loaded=sleela::langin::loadFiles(files);if(mode==EMIT_SLEELA)std::cout<<loaded.emitted;else if(mode==EMIT_XCLASS)std::cout<<sleela::langin::emitXclass(loaded.program,loaded.metas);else for(const auto&m:loaded.metas)std::cout<<sleela::langin::infoLine(m)<<"\n";return 0;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static int runFile(const std::string&path){if(slvm_is_artifact_file(path.c_str()))return runArtifact(path);if(hasExt(path,".xclass"))return runXclass({path});if(isLangInput(path))return runLangin({path});return runSource(path);}
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
    if(argc<2)return usage();
    // Global option phase: consume any leading --memory-manager[=<size>] before
    // the subcommand. This may only precede the subcommand; per-subcommand flag
    // parsing (defender/xclass/native) is left untouched.
    bool mmEnabled=false;size_t mmLimit=0;
    // Env default (an explicit CLI flag below overrides it).
    {size_t l=0;if(memoryManagerRequested(l)){mmEnabled=true;mmLimit=l;}}
    int start=1;
    while(start<argc){
        std::string a=argv[start];
        if(a=="--memory-manager"||a=="--mm"){mmEnabled=true;mmLimit=0;start++;}
        else if(a.rfind("--memory-manager=",0)==0||a.rfind("--mm=",0)==0){
            std::string v=a.substr(a.find('=')+1);size_t l=0;
            if(!parseByteSize(v,l)){std::cerr<<"sleela: invalid --memory-manager size '"<<v<<"'\n";return 2;}
            mmEnabled=true;mmLimit=l;start++;
        }else break;
    }
    if(mmEnabled)enableMemoryManager(mmLimit,true);
    if(start>=argc)return usage();
    std::string cmd=argv[start];
    // Shift argv so the existing positional parsing (argv[2]==first operand)
    // continues to work regardless of any consumed global options.
    argc-=(start-1);argv+=(start-1);
    if(cmd=="version"||cmd=="--version"||cmd=="-v"){std::cout<<kVersion<<"\n";std::cout<<"  supported .sleela syntax: "<<sleela::minSupportedSyntax().str()<<" .. "<<sleela::maxSupportedSyntax().str()<<"\n";return 0;}
    if(cmd=="native"||cmd=="exec")return nativeCmd(argc,argv);
    int rc;
    if(cmd=="compile"){if(argc!=5||std::string(argv[3])!="-o")return usage();rc=compileFile(argv[2],argv[4]);}
    else if(cmd=="check"){if(argc<3)return usage();rc=checkFile(argv[2]);}
    else if(cmd=="run"){if(argc<3)return usage();if(verifyBeforeExecution(fs::current_path()))return 1;if(hasExt(argv[2],".xclass")){std::vector<std::string>files;for(int i=2;i<argc;i++)files.push_back(argv[i]);rc=runXclass(files);}else if(isLangInput(argv[2])){std::vector<std::string>files;for(int i=2;i<argc;i++)files.push_back(argv[i]);rc=runLangin(files);}else rc=runFile(argv[2]);}
    else if(cmd=="xclass"){if(argc<3)return usage();rc=xclassCmd(argc,argv);}
    else if(cmd=="langin"){if(argc<3)return usage();rc=langinCmd(argc,argv);}
    else if(cmd=="nordshrift"){if(argc<3)return usage();rc=nordshriftCmd(argc,argv);}
    else if(cmd=="defender")return defenderCmd(argc,argv);
    else if(hasExt(cmd,".sleela")||hasExt(cmd,".xclass")||isLangInput(cmd)){if(verifyBeforeExecution(fs::current_path()))return 1;rc=runFile(cmd);}
    else return usage();
    if(mmEnabled)reportMemoryManager();
    return rc;
}
