#include "debugger_backend_linux.hpp"
#if defined(__linux__)
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

namespace sleela::debugger {
class LinuxPtraceBackend final : public DebugBackend {
    pid_t pid_ = -1;
    bool attached_ = false;
public:
    BackendKind kind() const noexcept override { return BackendKind::LinuxPtrace; }
    BackendCapabilities capabilities() const noexcept override {
        BackendCapabilities c;
        c.launch=true; c.attach=true; c.continue_execution=true; c.step=true;
        c.threads=true; c.memory=true; c.exceptions=true;
        return c;
    }
    bool launch(const BackendRequest& r, std::string& e) override {
        if (r.executable.empty()) { e="executable is required"; return false; }
        pid_t child=fork();
        if(child<0){e=std::strerror(errno);return false;}
        if(child==0){
            if(ptrace(PTRACE_TRACEME,0,nullptr,nullptr)<0)_exit(127);
            std::vector<char*> argv; argv.push_back(const_cast<char*>(r.executable.c_str()));
            for(const auto& a:r.arguments) argv.push_back(const_cast<char*>(a.c_str()));
            argv.push_back(nullptr);
            execv(r.executable.c_str(),argv.data()); _exit(127);
        }
        pid_=child;
        if(waitpid(pid_,nullptr,0)<0){e=std::strerror(errno);return false;}
        attached_=true; return true;
    }
    bool attach(const BackendRequest& r,std::string& e) override {
        if(r.process_id.empty()){e="process_id is required";return false;}
        pid_=static_cast<pid_t>(std::stol(r.process_id));
        if(ptrace(PTRACE_ATTACH,pid_,nullptr,nullptr)<0){e=std::strerror(errno);return false;}
        if(waitpid(pid_,nullptr,0)<0){e=std::strerror(errno);return false;}
        attached_=true; return true;
    }
    bool resume(std::string& e) override {
        if(!attached_){e="no traced process";return false;}
        if(ptrace(PTRACE_CONT,pid_,nullptr,nullptr)<0){e=std::strerror(errno);return false;}
        return true;
    }
    bool step(bool over,std::string& e) override {
        (void)over;
        if(!attached_){e="no traced process";return false;}
        if(ptrace(PTRACE_SINGLESTEP,pid_,nullptr,nullptr)<0){e=std::strerror(errno);return false;}
        return true;
    }
    bool setLineStop(const SourceLocation&,std::string& e) override {
        e="source-line to instruction mapping requires debug symbols/native symbol layer"; return false;
    }
    bool poll(DebugEvent& event,std::string& e) override {
        if(!attached_){e="no traced process";return false;}
        int status=0; pid_t r=waitpid(pid_,&status,WNOHANG);
        if(r<0){e=std::strerror(errno);return false;}
        if(r==0)return false;
        event.type=WIFEXITED(status)?EventType::Crash:EventType::Exception;
        event.message=WIFEXITED(status)?"process exited":"process stopped";
        return true;
    }
};
std::unique_ptr<DebugBackend> makeLinuxPtraceBackend(){return std::make_unique<LinuxPtraceBackend>();}
}
#else
namespace sleela::debugger { std::unique_ptr<DebugBackend> makeLinuxPtraceBackend(){return makePortableBackend();} }
#endif
