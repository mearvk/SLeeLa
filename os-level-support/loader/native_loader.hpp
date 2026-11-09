#ifndef SLEELA_NATIVE_LOADER_HPP
#define SLEELA_NATIVE_LOADER_HPP

#include "native_loader.h"
#include <string>
#include <vector>

namespace sleela {

class NativeModule {
public:
    NativeModule() noexcept = default;
    ~NativeModule();
    NativeModule(const NativeModule &) = delete;
    NativeModule &operator=(const NativeModule &) = delete;
    NativeModule(NativeModule &&other) noexcept;
    NativeModule &operator=(NativeModule &&other) noexcept;

    SLNativeStatus open(const std::string &path);
    SLNativeStatus run(const std::vector<std::string> &args, int &result);
    SLNativeStatus call(const std::string &method,
                        const std::vector<SLNativeValue> &args,
                        SLNativeValue &result);
    SLNativeStatus close() noexcept;
    bool is_open() const noexcept { return module_.handle != nullptr; }

private:
    SLNativeModule module_{};
};

} // namespace sleela
#endif
