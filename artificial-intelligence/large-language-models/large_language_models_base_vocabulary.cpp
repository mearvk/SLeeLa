#include "large_language_models_base_vocabulary.hpp"
#include <string>
namespace sleela::ai {

BaseSpecials seed_default_base_vocabulary_handle(sllmv_vocabulary* handle){
 sllmbv_specials raw;
 sllmbv_status st = sllmbv_load_default(handle, &raw);
 if(st != SLLMBV_OK){
  throw std::runtime_error(std::string("base vocabulary seed failed: ") + sllmbv_status_string(st));
 }
 return BaseSpecials(raw);
}

BaseSpecials seed_default_base_vocabulary(LanguageModelVocabulary& vocab){
 return seed_default_base_vocabulary_handle(vocab.native_handle());
}

} // namespace sleela::ai
