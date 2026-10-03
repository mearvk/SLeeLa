#include "slvm_os.hpp"
#if defined(_WIN32)
namespace sleela::vm { class windows_adapter final : public os_adapter { public: const char* name() const noexcept override{return "Windows";} os_result probe() const override{return {os_status::ok,0,"platform-probe"};} }; }
#endif
