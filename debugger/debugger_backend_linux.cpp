#include "debugger_backend_linux.hpp"
#if defined(__linux__)
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <vector>
namespace sleela::debugger {
class LinuxPtraceBackend final : public DebugBackend {
    pid_t pid_=-1; bool attached_=false;
public:
    BackendKind kind() const noexcept override { return BackendKind::LinuxPtrace; }
    BackendCapabilities capabilities() const noexcept override {
        BackendCapabilities c; c.launch=true;c.attach=true;c.continue_execution=true;c.step=true;c.threads=true;c.memory=true;c.registers=true;c.exceptions=true; return c;
    }
    bool launch(const BackendRequest& r,std::string& e) override {
        if(r.executable.empty()){e="executable is required";return false;} pid_t child=fork();
        if(child<0){e=std::strerror(errno);return false;}
        if(child==0){if(ptrace(PTRACE_TRACEME,0,nullptr,nullptr)<0)_exit(127);std::vector<char*> argv;argv.push_back(const_cast<char*>(r.executable.c_str()));for(const auto&a:r.arguments)argv.push_back(const_cast<char*>(a.c_str()));argv.push_back(nullptr);execv(r.executable.c_str(),argv.data());_exit(127);}
        pid_=child;if(waitpid(pid_,nullptr,0)<0){e=std::strerror(errno);return false;}attached_=true;return true;
    }
    bool attach(const BackendRequest&r,std::string&e) override {
        if(r.process_id.empty()){e="process_id is required";return false;} try{pid_=static_cast<pid_t>(std::stol(r.process_id));}catch(const std::exception&){e="invalid process_id";return false;}
        if(pid_<=0){e="invalid process_id";return false;} if(ptrace(PTRACE_ATTACH,pid_,nullptr,nullptr)<0){e=std::strerror(errno);return false;} if(waitpid(pid_,nullptr,0)<0){e=std::strerror(errno);return false;}attached_=true;return true;
    }
    bool resume(std::string&e) override {if(!attached_){e="no traced process";return false;}if(ptrace(PTRACE_CONT,pid_,nullptr,nullptr)<0){e=std::strerror(errno);return false;}return true;}
    bool step(bool over,std::string&e) override {(void)over;if(!attached_){e="no traced process";return false;}if(ptrace(PTRACE_SINGLESTEP,pid_,nullptr,nullptr)<0){e=std::strerror(errno);return false;}return true;}
    bool readMemory(std::uint64_t address,void*buffer,std::size_t size,std::string&e) override {
        if(!attached_||!buffer||size==0){e="invalid memory read request";return false;}unsigned char*out=static_cast<unsigned char*>(buffer);
        for(std::size_t off=0;off<size;off+=sizeof(long)){errno=0;long word=ptrace(PTRACE_PEEKDATA,pid_,reinterpret_cast<void*>(address+off),nullptr);if(errno){e=std::strerror(errno);return false;}std::size_t n=sizeof(word);if(n>size-off)n=size-off;std::memcpy(out+off,&word,n);}return true;
    }
    bool writeMemory(std::uint64_t address,const void*buffer,std::size_t size,std::string&e) override {
        if(!attached_||!buffer||size==0){e="invalid memory write request";return false;}const unsigned char*in=static_cast<const unsigned char*>(buffer);
        for(std::size_t off=0;off<size;off+=sizeof(long)){errno=0;long word=ptrace(PTRACE_PEEKDATA,pid_,reinterpret_cast<void*>(address+off),nullptr);if(errno){e=std::strerror(errno);return false;}std::size_t n=sizeof(word);if(n>size-off)n=size-off;std::memcpy(&word,in+off,n);if(ptrace(PTRACE_POKEDATA,pid_,reinterpret_cast<void*>(address+off),reinterpret_cast<void*>(word))<0){e=std::strerror(errno);return false;}}return true;
    }
    bool readRegisters(BackendRegisterSnapshot&out,std::string&e) override {
        if(!attached_){e="no traced process";return false;}
#if defined(__x86_64__)
        struct user_regs_struct regs{};if(ptrace(PTRACE_GETREGS,pid_,nullptr,&regs)<0){e=std::strerror(errno);return false;}out.architecture="x86_64";out.instruction_pointer=regs.rip;out.stack_pointer=regs.rsp;out.frame_pointer=regs.rbp;return true;
#else
        e="register snapshot currently implemented for x86_64 Linux";return false;
#endif
    }
    bool setLineStop(const SourceLocation&,std::string&e) override {e="source-line to instruction mapping requires debug symbols/native symbol layer";return false;}
    bool poll(DebugEvent&event,std::string&e) override {
        if(!attached_){e="no traced process";return false;}int status=0;pid_t r=waitpid(pid_,&status,WNOHANG|__WALL);if(r<0){e=std::strerror(errno);return false;}if(r==0)return false;
        if(WIFEXITED(status)){attached_=false;event.type=EventType::Info;event.message="process exited normally";return true;}
        if(WIFSIGNALED(status)){attached_=false;event.type=EventType::Crash;event.message="process terminated by signal "+std::to_string(WTERMSIG(status));return true;}
        event.type=EventType::Exception;event.message=WIFSTOPPED(status)?"process stopped":"process state changed";return true;
    }
};
std::unique_ptr<DebugBackend> makeLinuxPtraceBackend(){return std::make_unique<LinuxPtraceBackend>();}
}
#else
namespace sleela::debugger { std::unique_ptr<DebugBackend> makeLinuxPtraceBackend(){return makePortableBackend();} }
#endif