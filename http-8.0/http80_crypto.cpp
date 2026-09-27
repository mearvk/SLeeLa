/* Module path mirror of http/8.0/http80_crypto.cpp; canonical source remains http/8.0/http80_crypto.cpp. */
#include "http80_crypto.hpp"
namespace http80 {
bool CryptoProfile::valid() const { return http80_crypto_validate_transition(key_agreement.c_str(),kem.c_str(),kdf.c_str(),aead.c_str(),signature.c_str())!=0; }
std::vector<std::string> supported_crypto_names(){const http80_crypto_algorithm *p=nullptr;size_t n=http80_crypto_catalog(&p);std::vector<std::string> v;for(size_t i=0;i<n;i++)v.emplace_back(p[i].name);return v;}
bool select_crypto(const std::string& n,http80_crypto_selection& s){return http80_crypto_select(n.c_str(),&s)!=0;}
}
