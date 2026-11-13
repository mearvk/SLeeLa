// ===========================================================================
// java_host.cpp -- the Java execution host (JVM hand-off) for `sleela run`.
// ===========================================================================
// Runs `.java` sources, `.jar` archives and `--class-path/-cp/--class` main
// classes on an external JVM discovered by runtime/java_runtime_bridge. It is a
// LanguageHandler used only for its runHost hook and registers itself, so the
// compiler driver contains no JVM launch code: it just offers the `run`
// arguments to lang::runHost. It is a separate object from java_handler.cpp
// because it depends on the runtime bridge, which the compile-only Nordshrift
// binary does not link. See impl/frontend/COMPILER-JAVA-REVIEW.md (Stage 2).
// ===========================================================================
#include "../language_handler.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#if defined(_WIN32)
#  include <process.h>
#  include <windows.h>
#else
#  include <sys/wait.h>
#  include <unistd.h>
#endif
extern "C" {
#include "../../../runtime/java_runtime_bridge.h"
}
namespace fs=std::filesystem;
namespace sleela {
namespace javahost {
static bool readFile(const std::string&path,std::string&out){std::ifstream f(path,std::ios::binary);if(!f)return false;std::ostringstream ss;ss<<f.rdbuf();out=ss.str();return true;}
static bool hasExt(const std::string& path,const std::string& ext){return path.size()>=ext.size()&&path.compare(path.size()-ext.size(),ext.size(),ext)==0;}
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
    if(!plan.ready || !probe.java_executable[0]){
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
                        "SLEELA_JAVAFX_MODULE_PATH and optionally SLEELA_JAVAFX_MODULES.\n";
        }
#if !defined(_WIN32) && !defined(__APPLE__)
        const char* display=std::getenv("DISPLAY");
        const char* wayland=std::getenv("WAYLAND_DISPLAY");
        if((!display||!*display)&&(!wayland||!*wayland))
            std::cerr<<"[javafx] no DISPLAY or WAYLAND_DISPLAY detected; graphical startup may fail on this headless host.\n";
#endif
    }
    /* Optional JVM options passthrough for single-file source launches, e.g.
       SLEELA_JAVA_OPTS="--enable-preview --source 21 -ea". These are JVM flags
       and must precede the source-file operand. Tokens are split on unquoted
       whitespace; there is no shell, so each token becomes one argv element. */
    if(const char* jopts=std::getenv("SLEELA_JAVA_OPTS")){
        std::string tok; std::string s=jopts;
        for(size_t i=0;i<=s.size();++i){
            char c=(i<s.size())?s[i]:' ';
            if(c==' '||c=='\t'||c=='\n'||c=='\r'){ if(!tok.empty()){args.push_back(tok);tok.clear();} }
            else tok.push_back(c);
        }
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

class JavaHost : public LanguageHandler {
public:
    const char* name() const override { return "java-host"; }
    // args[0] is the file or option after `run`; the rest are program arguments.
    bool runHost(const std::vector<std::string>& a,int& rc) const override {
        if(a.empty()) return false;
        const std::string& first=a[0];
        auto rest=[&](size_t from){ return std::vector<std::string>(a.size()>from?a.begin()+from:a.end(),a.end()); };
        if(hasExt(first,".jar")){ rc=runJavaJar(first,rest(1)); return true; }
        if(first=="--class-path"||first=="--classpath"||first=="-cp"){
            if(a.size()<3){ std::cerr<<"sleela run: expected --class-path <path> <MainClass> [arguments...]\n"; rc=2; return true; }
            rc=runJavaClass(a[1],a[2],rest(3)); return true;
        }
        if(first=="--class"){
            if(a.size()<2){ std::cerr<<"sleela run: expected --class <MainClass> [arguments...]\n"; rc=2; return true; }
            rc=runJavaClass(".",a[1],rest(2)); return true;
        }
        if(hasExt(first,".java")){
            std::string src;
            if(!readFile(first,src)) return false;          // not readable: the core ingests it
            SleelaJavaRuntimeProbeResult probe{};
            SleelaJavaProgramRequest request{};
            request.classpath=".";
            request.source_file=first.c_str();
            SleelaJavaProgramPlan plan{};
            int dispatch=sleela_java_runtime_dispatch_source(src.data(),src.size(),first.c_str(),&request,&probe,&plan);
            if(dispatch<0){ rc=1; return true; }
            rc=runJavaFrameworkSource(first,rest(1)); return true;
        }
        return false;
    }
};
static JavaHost g_host;
static lang::Registrar g_registrar(&g_host);
}
}
