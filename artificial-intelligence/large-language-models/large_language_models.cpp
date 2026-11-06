#include "large_language_models.hpp"

#include <exception>
#include <utility>

namespace sleela::ai {
namespace {
struct CallbackContext {
    const std::function<bool(std::string_view)>* callback;
    std::exception_ptr exception;
};

int forward_chunk(const char* bytes, std::size_t length, void* opaque) {
    auto* context = static_cast<CallbackContext*>(opaque);
    if (!context || !context->callback || !*context->callback) return 1;
    try {
        return (*context->callback)(std::string_view(bytes ? bytes : "", length)) ? 1 : 0;
    } catch (...) {
        context->exception = std::current_exception();
        return 0;
    }
}
} // namespace

void LanguageModel::throw_if_error(sllm_status status) {
    if (status != SLLM_OK) throw std::runtime_error(sllm_status_string(status));
}

LanguageModel::LanguageModel(const sllm_backend& backend) {
    throw_if_error(sllm_engine_create(&backend, &handle_));
}

LanguageModel::~LanguageModel() { sllm_engine_destroy(handle_); }

LanguageModel::LanguageModel(LanguageModel&& other) noexcept
    : handle_(std::exchange(other.handle_, nullptr)) {}

LanguageModel& LanguageModel::operator=(LanguageModel&& other) noexcept {
    if (this != &other) {
        sllm_engine_destroy(handle_);
        handle_ = std::exchange(other.handle_, nullptr);
    }
    return *this;
}

void LanguageModel::load(const std::string& model_id) {
    if (!handle_) throw std::logic_error("LanguageModel is empty or moved-from");
    throw_if_error(sllm_engine_load(handle_, model_id.c_str()));
}

void LanguageModel::unload() noexcept { sllm_engine_unload(handle_); }
bool LanguageModel::is_loaded() const noexcept { return sllm_engine_is_loaded(handle_) != 0; }

std::string LanguageModel::backend_name() const {
    if (!handle_) throw std::logic_error("LanguageModel is empty or moved-from");
    const char* name = sllm_engine_backend_name(handle_);
    return name ? name : std::string{};
}

void LanguageModel::generate(const std::string& prompt,
                             const GenerationOptions& options,
                             const std::function<bool(std::string_view)>& callback) {
    if (!handle_) throw std::logic_error("LanguageModel is empty or moved-from");
    sllm_generation_options c_options{};
    c_options.abi_version = SLLM_ABI_VERSION;
    c_options.max_new_tokens = options.max_new_tokens;
    c_options.temperature = options.temperature;
    c_options.top_p = options.top_p;
    c_options.seed = options.seed;
    CallbackContext context{&callback, nullptr};
    const sllm_status status = sllm_engine_generate(
        handle_, prompt.c_str(), &c_options, forward_chunk, &context);
    if (context.exception) std::rethrow_exception(context.exception);
    throw_if_error(status);
}

} // namespace sleela::ai
