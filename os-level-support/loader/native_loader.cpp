#include "native_loader.hpp"

#include <utility>

namespace sleela {

NativeModule::~NativeModule() { close(); }

NativeModule::NativeModule(NativeModule &&other) noexcept : module_(other.module_) {
    other.module_ = {};
}

NativeModule &NativeModule::operator=(NativeModule &&other) noexcept {
    if (this != &other) {
        close();
        module_ = other.module_;
        other.module_ = {};
    }
    return *this;
}

SLNativeStatus NativeModule::open(const std::string &path) {
    return sl_native_open(&module_, path.c_str());
}

SLNativeStatus NativeModule::run(const std::vector<std::string> &args, int &result) {
    std::vector<const char *> argv;
    argv.reserve(args.size());
    for (const auto &arg : args) argv.push_back(arg.c_str());
    return sl_native_run_module(&module_, static_cast<int>(argv.size()),
                                argv.empty() ? nullptr : argv.data(), &result);
}

SLNativeStatus NativeModule::close() noexcept {
    return sl_native_close(&module_);
}

} // namespace sleela
