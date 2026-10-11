#ifndef SLEELA_JSON_CONNECTOR_HPP
#define SLEELA_JSON_CONNECTOR_HPP
#include "json_connector.h"
#include <stdexcept>
#include <string>
#include <vector>
namespace sleela::connector {
inline std::vector<sleela_json_token> tokenizeJson(const std::string& text){std::vector<sleela_json_token> t(text.size()+1);size_t n=sleela_json_tokenize(text.c_str(),t.data(),t.size());if(!n)throw std::runtime_error("invalid JSON structure or token capacity exceeded");t.resize(n);return t;}
}
#endif
