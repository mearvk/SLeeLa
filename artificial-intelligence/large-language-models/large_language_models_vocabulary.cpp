#include "large_language_models_vocabulary.hpp"
#include <stdexcept>
#include <utility>
namespace sleela::ai {
void LanguageModelVocabulary::check(sllmv_status s){if(s==SLLMV_OK)return;const char* message=s==SLLMV_NOT_FOUND?"token not found":s==SLLMV_OUT_OF_MEMORY?"vocabulary allocation failed":s==SLLMV_INVALID_ARGUMENT?"invalid vocabulary argument":"vocabulary operation failed";throw std::runtime_error(message);}
LanguageModelVocabulary::LanguageModelVocabulary(){check(sllmv_vocabulary_create(&handle_));}
LanguageModelVocabulary::~LanguageModelVocabulary(){sllmv_vocabulary_destroy(handle_);}
LanguageModelVocabulary::LanguageModelVocabulary(LanguageModelVocabulary&& x) noexcept:handle_(std::exchange(x.handle_,nullptr)){}
LanguageModelVocabulary& LanguageModelVocabulary::operator=(LanguageModelVocabulary&& x) noexcept{if(this!=&x){sllmv_vocabulary_destroy(handle_);handle_=std::exchange(x.handle_,nullptr);}return *this;}
std::uint32_t LanguageModelVocabulary::add(std::string_view value){if(!handle_||value.empty()||value.find('\0')!=std::string_view::npos)throw std::invalid_argument("token must be a non-empty NUL-free string");std::string s(value);std::uint32_t id=0;check(sllmv_vocabulary_add(handle_,s.c_str(),&id));return id;}
std::uint32_t LanguageModelVocabulary::find(std::string_view value) const{if(!handle_||value.empty()||value.find('\0')!=std::string_view::npos)throw std::invalid_argument("token must be a non-empty NUL-free string");std::string s(value);std::uint32_t id=0;check(sllmv_vocabulary_find(handle_,s.c_str(),&id));return id;}
std::string LanguageModelVocabulary::token(std::uint32_t id) const{if(!handle_)throw std::logic_error("vocabulary is empty or moved-from");const char* value=nullptr;check(sllmv_vocabulary_token(handle_,id,&value));return value;}
std::vector<std::uint32_t> LanguageModelVocabulary::encode(std::string_view text) const{if(!handle_||text.find('\0')!=std::string_view::npos)throw std::invalid_argument("text must be NUL-free");std::string s(text);std::uint32_t* ids=nullptr;std::size_t count=0;check(sllmv_vocabulary_encode(handle_,s.c_str(),&ids,&count));std::vector<std::uint32_t> result;if(count)result.assign(ids,ids+count);sllmv_vocabulary_free_ids(ids);return result;}
std::size_t LanguageModelVocabulary::size() const noexcept{return sllmv_vocabulary_size(handle_);}
}
