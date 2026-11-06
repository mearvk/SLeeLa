#ifndef SLEELA_AI_LLM_VOCABULARY_HPP
#define SLEELA_AI_LLM_VOCABULARY_HPP
#include "large_language_models_vocabulary.h"
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
namespace sleela::ai {
class LanguageModelVocabulary {
public:
 LanguageModelVocabulary();
 ~LanguageModelVocabulary();
 LanguageModelVocabulary(const LanguageModelVocabulary&)=delete;
 LanguageModelVocabulary& operator=(const LanguageModelVocabulary&)=delete;
 LanguageModelVocabulary(LanguageModelVocabulary&&) noexcept;
 LanguageModelVocabulary& operator=(LanguageModelVocabulary&&) noexcept;
 std::uint32_t add(std::string_view token);
 std::uint32_t find(std::string_view token) const;
 std::string token(std::uint32_t id) const;
 std::vector<std::uint32_t> encode(std::string_view text) const;
 std::size_t size() const noexcept;
 // Borrowed native handle for interop with the C base-vocabulary/builder APIs.
 // Ownership stays with this object; do not destroy the returned handle.
 sllmv_vocabulary* native_handle() const noexcept { return handle_; }
private:
 sllmv_vocabulary* handle_=nullptr;
 static void check(sllmv_status status);
};
}
#endif
