#ifndef SLEELA_AI_LARGE_LANGUAGE_MODELS_HPP
#define SLEELA_AI_LARGE_LANGUAGE_MODELS_HPP

#include "large_language_models.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace sleela::ai {

struct GenerationOptions {
    std::uint32_t max_new_tokens = 256;
    double temperature = 0.7;
    double top_p = 0.95;
    std::uint64_t seed = 0;
};

class LanguageModel {
public:
    explicit LanguageModel(const sllm_backend& backend);
    ~LanguageModel();

    LanguageModel(const LanguageModel&) = delete;
    LanguageModel& operator=(const LanguageModel&) = delete;
    LanguageModel(LanguageModel&& other) noexcept;
    LanguageModel& operator=(LanguageModel&& other) noexcept;

    void load(const std::string& model_id);
    void unload() noexcept;
    bool is_loaded() const noexcept;
    std::string backend_name() const;

    /* Return false from on_chunk to request cancellation. */
    void generate(const std::string& prompt,
                  const GenerationOptions& options,
                  const std::function<bool(std::string_view)>& on_chunk);

private:
    sllm_engine* handle_ = nullptr;
    static void throw_if_error(sllm_status status);
};

} // namespace sleela::ai

#endif
