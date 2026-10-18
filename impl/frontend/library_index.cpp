// Library discovery implementation shared by compiler and Nordshrift.
#include "library_index.h"
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <algorithm>
#include <set>
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
namespace sleela { namespace library {
// The installed /lib tree, derived from the running executable's own location
// (<prefix>/bin/<cmd> -> <prefix>/lib/sleela/lib). This keeps the import
// validator and every other Index::discover() caller finding the SAME library
// root as the driver: an installed binary resolves /lib from its install
// location, not just from the current working directory. Returns empty when the
// executable cannot be located or the installed layout is not present.
static fs::path installedLibraryRoot(){
    std::error_code ec; fs::path exe;
#if defined(_WIN32)
    wchar_t buf[MAX_PATH]; DWORD n=GetModuleFileNameW(nullptr,buf,MAX_PATH);
    if(n==0||n>=MAX_PATH) return fs::path();
    exe=fs::path(std::wstring(buf,buf+n));
#elif defined(__APPLE__)
    char buf[PATH_MAX]; uint32_t size=sizeof(buf);
    if(_NSGetExecutablePath(buf,&size)!=0) return fs::path();
    exe=fs::path(buf);
#else
    char buf[PATH_MAX]; ssize_t n=::readlink("/proc/self/exe",buf,sizeof(buf)-1);
    if(n<=0) return fs::path();
    buf[n]='\0'; exe=fs::path(buf);
#endif
    fs::path c=fs::weakly_canonical(exe,ec); if(!ec) exe=c; else ec.clear();
    fs::path prefix=exe.parent_path().parent_path();   // <prefix>
    fs::path lib=prefix/"lib"/"sleela"/"lib";
    if(fs::is_directory(lib,ec)) return lib;
    return fs::path();
}
Index Index::discover(const std::string& preferredRoot){
 Index o; std::vector<fs::path> roots; if(!preferredRoot.empty())roots.emplace_back(preferredRoot);
 if(const char*e=std::getenv("SLEELA_LIB");e&&*e)roots.emplace_back(e);
 if(fs::path installed=installedLibraryRoot(); !installed.empty())roots.emplace_back(installed);
 roots.emplace_back("lib"); roots.emplace_back("../lib"); roots.emplace_back("../../lib");
 for(const auto&p:roots)if(fs::is_directory(p)){o.root_=fs::weakly_canonical(p).string();break;}
 if(o.root_.empty())return o;
 for(const auto&f:fs::recursive_directory_iterator(o.root_)){
  if(!f.is_regular_file() || f.path().extension()!=".sleela") continue;
  fs::path rel=fs::relative(f.path(),o.root_);
  std::string package="__root__";
  if(rel.has_parent_path()){
    auto it=rel.begin();
    if(it!=rel.end()) package=it->string();
  }
  o.symbols_.push_back({package,f.path().stem().string(),rel.generic_string()});
 }
 std::sort(o.symbols_.begin(),o.symbols_.end(),[](const Symbol&a,const Symbol&b){
   if(a.package!=b.package)return a.package<b.package;
   if(a.name!=b.name)return a.name<b.name;
   return a.path<b.path;
 }); return o;
}
bool Index::empty()const{return symbols_.empty();}
size_t Index::symbolCount()const{return symbols_.size();}
bool Index::hasPackage(const std::string&p)const{for(const auto&s:symbols_)if(s.package==p)return true;return false;}
size_t Index::packageCount()const{std::set<std::string> packages; for(const auto&s:symbols_) packages.insert(s.package); return packages.size();}
size_t Index::packageSymbolCount(const std::string&p)const{size_t n=0;for(const auto&s:symbols_)if(s.package==p)++n;return n;}
const Symbol* Index::findSymbol(const std::string&p,const std::string&n)const{for(const auto&s:symbols_)if(s.package==p&&s.name==n)return &s;return nullptr;}
const std::vector<Symbol>& Index::symbols()const{return symbols_;}
const std::string& Index::root()const{return root_;}
std::string Index::resolveImport(const std::string&i)const{return hasPackage(i)?root_+"/"+i:"";}
std::string Index::resolveSymbol(const std::string&p,const std::string&n)const{const Symbol*s=findSymbol(p,n);return s?root_+"/"+s->path:"";}
// Imports satisfied by a native/synthesized subject library rather than by a
// unit under /lib. These are lowered in the frontend (chemistry/financial have
// their own lowering; the rest are built by native::lowerProgram) and must not
// be required to exist as /lib packages.
static bool isNativeSubjectImport(const std::string&x){
    return x=="chemistry"||x=="financial"||x=="native"||
           x=="math"||x=="physics"||x=="economics"||x=="inference"||
           x=="astrophysics"||x=="sociology"||x=="excel"||x=="json"||
           x=="crypto"||x=="net";
}
void validateImports(const std::vector<std::string>&is,const Index&i){
 for(const auto&x:is){
  if(isNativeSubjectImport(x)||i.hasPackage(x)) continue;
  // Versioned source roots use dotted imports, e.g. gui.version-military.
  std::string rel=x;
  std::replace(rel.begin(),rel.end(),'.','/');
  if(rel.find('/')!=std::string::npos && fs::is_directory(fs::path(i.root())/rel)) continue;
  throw std::runtime_error("Library import '"+x+"' is not present under /lib");
 }
}
}}
