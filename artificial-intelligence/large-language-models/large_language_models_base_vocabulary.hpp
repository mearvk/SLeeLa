#ifndef SLEELA_AI_LLM_BASE_VOCABULARY_HPP
#define SLEELA_AI_LLM_BASE_VOCABULARY_HPP
#include "large_language_models_base_vocabulary.h"
#include "large_language_models_vocabulary.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
namespace sleela::ai {

// Resolved special-token IDs for a seeded base vocabulary. A role that was not
// declared reports std::nullopt rather than a sentinel.
class BaseSpecials {
public:
 explicit BaseSpecials(const sllmbv_specials& raw) : raw_(raw) {}
 std::optional<std::uint32_t> id(sllmbv_role role) const {
  std::uint32_t v = sllmbv_special_id(&raw_, role);
  if (v == SLLMBV_NO_ID) return std::nullopt;
  return v;
 }
 std::optional<std::uint32_t> pad()  const { return id(SLLMBV_ROLE_PAD); }
 std::optional<std::uint32_t> unk()  const { return id(SLLMBV_ROLE_UNK); }
 std::optional<std::uint32_t> bos()  const { return id(SLLMBV_ROLE_BOS); }
 std::optional<std::uint32_t> eos()  const { return id(SLLMBV_ROLE_EOS); }
 std::optional<std::uint32_t> sep()  const { return id(SLLMBV_ROLE_SEP); }
 std::optional<std::uint32_t> mask() const { return id(SLLMBV_ROLE_MASK); }
 const sllmbv_specials& raw() const noexcept { return raw_; }
private:
 sllmbv_specials raw_;
};

// Seed the compiled-in default base vocabulary (sleela-base-en-1.0) into an
// empty LanguageModelVocabulary. Throws std::runtime_error on any failure,
// including a non-empty target or an ID mismatch (fail closed).
BaseSpecials seed_default_base_vocabulary(LanguageModelVocabulary& vocab);

// Low-level: operate directly on the C handle. Kept minimal to preserve the
// opaque handle; ownership of the handle stays with the caller.
BaseSpecials seed_default_base_vocabulary_handle(sllmv_vocabulary* handle);

} // namespace sleela::ai
#endif
