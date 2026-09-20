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
}
namespace fs=std::filesystem;
static const char* kVersion="Sleelvac™ 1.4 (Sleela compiler; executable native math/physics/economics/chemistry/financial modules; persistent .sleela Core artifacts; .xclass input; OS Defender provisioning; SHA-256 execution gate)";
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
static int verifyBeforeExecution(const fs::path&root){
    const char* env=std::getenv("SLEELA_SHA256_MANIFEST");
    if(!env||!*env){
        std::cerr<<"sleelvac: SHA-256 verification is required; set SLEELA_SHA256_MANIFEST to a trusted JSON manifest\n";
        return 1;
    }
    fs::path manifest=fs::path(env);
    if(!manifest.is_absolute())manifest=root/manifest;
    fs::path tool=root/"tools"/"verify-before-execution.py";
    if(!fs::exists(tool)){
        std::cerr<<"sleelvac: verification tool not found: "<<tool<<"\n";
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
#else
    return "https://github.com/mearvk/Linux.Admin.Defender";
#endif
}
static const char* defenderName(){
#ifdef _WIN32
    return "Windows.Admin.Defender";
#else
    return "Linux.Admin.Defender";
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
    if(action=="fetch")return defenderFetch(base,opts);
    if(action=="build"){if(!fs::exists(source)&&defenderFetch(base,opts))return 1;if(verifyBeforeExecution(fs::current_path()))return 1;return defenderBuild(source);}
    if(action=="install"){if(!fs::exists(source)&&defenderFetch(base,opts))return 1;if(verifyBeforeExecution(fs::current_path()))return 1;if(defenderBuild(source))return 1;return defenderInstall(source,opts);}
    /*provision*/{if(defenderFetch(base,opts))return 1;if(verifyBeforeExecution(fs::current_path()))return 1;if(defenderBuild(source))return 1;return defenderInstall(source,opts);}
}
static void lowerNativeModules(sleela::Program& prog){sleela::chemistry::lowerProgram(prog);sleela::financial::lowerProgram(prog);auto saved=prog.imports;prog.imports.erase(std::remove(prog.imports.begin(),prog.imports.end(),"chemistry"),prog.imports.end());prog.imports.erase(std::remove(prog.imports.begin(),prog.imports.end(),"financial"),prog.imports.end());sleela::native::lowerProgram(prog);prog.imports=std::move(saved);}
static int compileAndRun(sleela::Program& prog,const catalog::Catalog& cat,const sleela::SyntaxVersion& syntax={1,0}){SLVM*vm=slvm_new();if(!vm){std::cerr<<"sleelvac: unable to allocate Sleela VM\n";return 1;}int rc=0;try{lowerNativeModules(prog);sleela::compile(prog,vm,&cat,syntax);SLResult r=slvm_run(vm);if(r==SLR_ERROR){const char*e=slvm_error(vm);std::cerr<<"sleelvac: runtime error: "<<(e?e:"unknown")<<"\n";rc=1;}}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";rc=1;}slvm_free(vm);return rc;}
static catalog::Catalog loadCatalog(){const char*env=std::getenv("SLEELA_SHEET");const char*candidates[]={env,"SHEET.sheet","../SHEET.sheet","../../SHEET.sheet","../../../SHEET.sheet"};for(const char*p:candidates){if(!p||!*p)continue;bool ok=false;catalog::Catalog c=catalog::parseCatalogFile(p,&ok);if(ok)return c;}return catalog::Catalog{};}
static int usage(){std::cerr<<"Usage:\n  sleela compile <file.sleela> -o <program.sleela>\n  sleela run <file.sleela>\n  sleela run <program.sleela>\n  sleela run <file.xclass> [more...]\n  sleela xclass [--run|--emit|--info] <file.xclass> [more...]\n  sleela check <file.sleela>\n  sleela version\n  sleela defender detect\n  sleela defender <fetch|build|install|provision> [directory] --allow-defender --sha256 <hex> [--allow-root]\n\nSecurity:\n  SLEELA_SHA256_MANIFEST=<trusted JSON manifest> is required before compile, run, check, xclass, and Defender diagnostics/build/install/provision.\n  Defender fetch/build/install/provision additionally require --allow-defender (opt-in), --sha256 <hex> (payload integrity), and --allow-root for the privileged install step.\n";return 2;}
static bool checkSyntaxVersion(const std::string& path,const std::string& src){sleela::VersionResolution v=sleela::resolveSyntaxVersion(src);if(v.isError()){std::cerr<<"sleelvac: "<<path<<": error: "<<v.message<<"\n";return false;}if(v.isWarning())std::cerr<<"sleelvac: "<<path<<": warning: "<<v.message<<"\n";return true;}
static bool parseSource(const std::string&path,std::string&src,sleela::Program&prog,sleela::VersionResolution&version){if(!readFile(path,src)){std::cerr<<"sleelvac: cannot open '"<<path<<"'\n";return false;}if(!checkSyntaxVersion(path,src))return false;version=sleela::resolveSyntaxVersion(src);try{sleela::Lexer lexer(src);auto tokens=lexer.tokenize();sleela::Parser parser(std::move(tokens));prog=parser.parseProgram();sleela::Program validation;validation.imports=prog.imports;validation.imports.erase(std::remove(validation.imports.begin(),validation.imports.end(),"chemistry"),validation.imports.end());validation.imports.erase(std::remove(validation.imports.begin(),validation.imports.end(),"financial"),validation.imports.end());sleela::native::validateImports(validation);return true;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<path<<": "<<ex.what()<<"\n";return false;}}
static int checkFile(const std::string&path){if(verifyBeforeExecution(fs::current_path()))return 1;std::string src;sleela::Program prog;sleela::VersionResolution v;if(!parseSource(path,src,prog,v))return 1;try{sleela::chemistry::lowerProgram(prog);sleela::financial::lowerProgram(prog);}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<path<<": "<<ex.what()<<"\n";return 1;}std::cout<<path<<": ok (syntax "<<(v.pragmaPresent?"declared ":"assumed ")<<v.declared.str()<<")\n";return 0;}
static int compileFile(const std::string&sourcePath,const std::string&outputPath){if(verifyBeforeExecution(fs::current_path()))return 1;std::string src;sleela::Program prog;sleela::VersionResolution syntax;if(!parseSource(sourcePath,src,prog,syntax))return 1;catalog::Catalog cat=loadCatalog();try{int rc=sleela::compileToArtifact(prog,outputPath,&cat,syntax.declared);if(rc!=0)return 1;std::cout<<"sleelvac: "<<sourcePath<<" -> "<<outputPath<<" (runnable Sleela Core artifact)\n";return 0;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static int runArtifact(const std::string&path){SLVM*vm=slvm_load_file(path.c_str());if(!vm){std::cerr<<"sleelvac: cannot load runnable .sleela artifact '"<<path<<"'\n";return 1;}SLResult r=slvm_run(vm);if(r==SLR_ERROR){const char*e=slvm_error(vm);std::cerr<<"sleelvac: runtime error: "<<(e?e:"unknown")<<"\n";slvm_free(vm);return 1;}slvm_free(vm);return 0;}
static int runSource(const std::string&path){std::string src;sleela::Program prog;sleela::VersionResolution syntax;if(!parseSource(path,src,prog,syntax))return 1;catalog::Catalog cat=loadCatalog();return compileAndRun(prog,cat,syntax.declared);}
static int runXclass(const std::vector<std::string>&paths){catalog::Catalog cat=loadCatalog();try{sleela::xclass::Loaded loaded=sleela::xclass::loadFiles(paths);std::cout<<"[xclass] ingested "<<loaded.metas.size()<<" SecureJDK 28 class(es):\n";for(const auto&m:loaded.metas)std::cout<<"[xclass]   "<<sleela::xclass::infoLine(m)<<"\n";return compileAndRun(loaded.program,cat);}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static int xclassCmd(int argc,char**argv){enum{RUN,EMIT,INFO}mode=RUN;std::vector<std::string>files;for(int i=2;i<argc;i++){std::string a=argv[i];if(a=="--emit")mode=EMIT;else if(a=="--info")mode=INFO;else if(a=="--run")mode=RUN;else if(a.rfind("--",0)==0){std::cerr<<"sleelvac: unknown option "<<a<<"\n";return 2;}else files.push_back(a);}if(files.empty()){std::cerr<<"sleela xclass: no .xclass files given\n";return 2;}if(verifyBeforeExecution(fs::current_path()))return 1;if(mode==RUN)return runXclass(files);try{sleela::xclass::Loaded loaded=sleela::xclass::loadFiles(files);if(mode==EMIT)std::cout<<loaded.emitted;else for(const auto&m:loaded.metas){std::cout<<sleela::xclass::infoLine(m)<<"\n";if(!m.sourceFile.empty())std::cout<<"  source    : "<<m.sourceFile<<"\n";if(!m.edition.empty())std::cout<<"  edition   : "<<m.edition<<"\n";if(!m.signatureHex.empty())std::cout<<"  signature : "<<m.signatureAlg<<":"<<m.signatureHex<<(m.signed_?" (signed)":"")<<"\n";}return 0;}catch(const std::exception&ex){std::cerr<<"sleelvac: "<<ex.what()<<"\n";return 1;}}
static bool readFile(const std::string&path,std::string&out){std::ifstream f(path,std::ios::binary);if(!f)return false;std::ostringstream ss;ss<<f.rdbuf();out=ss.str();return true;}
static int runFile(const std::string&path){if(slvm_is_artifact_file(path.c_str()))return runArtifact(path);if(hasExt(path,".xclass"))return runXclass({path});return runSource(path);}
int main(int argc,char**argv){if(argc<2)return usage();std::string cmd=argv[1];if(cmd=="version"||cmd=="--version"||cmd=="-v"){std::cout<<kVersion<<"\n";std::cout<<"  supported .sleela syntax: "<<sleela::minSupportedSyntax().str()<<" .. "<<sleela::maxSupportedSyntax().str()<<"\n";return 0;}if(cmd=="compile"){if(argc!=5||std::string(argv[3])!="-o")return usage();return compileFile(argv[2],argv[4]);}if(cmd=="check"){if(argc<3)return usage();return checkFile(argv[2]);}if(cmd=="run"){if(argc<3)return usage();if(verifyBeforeExecution(fs::current_path()))return 1;if(hasExt(argv[2],".xclass")){std::vector<std::string>files;for(int i=2;i<argc;i++)files.push_back(argv[i]);return runXclass(files);}return runFile(argv[2]);}if(cmd=="xclass"){if(argc<3)return usage();return xclassCmd(argc,argv);}if(cmd=="defender")return defenderCmd(argc,argv);if(hasExt(cmd,".sleela")||hasExt(cmd,".xclass")){if(verifyBeforeExecution(fs::current_path()))return 1;return runFile(cmd);}return usage();}
