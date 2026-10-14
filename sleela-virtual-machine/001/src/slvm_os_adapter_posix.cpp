#include "slvm_os.hpp"
#if defined(__unix__) || defined(__APPLE__)
namespace sleela::vm { class posix_adapter final : public os_adapter { public: const char* name() const noexcept override{return "POSIX";} os_result probe() const override{return {os_status::ok,0,"platform-probe"};} }; }
#endif
