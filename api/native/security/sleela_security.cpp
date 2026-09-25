#include "sleela_security.hpp"
#include <openssl/sha.h>
#include <algorithm>
#include <cstring>
namespace sleela::security {
void secureZero(void*p,std::size_t n)noexcept{volatile std::uint8_t*q=static_cast<volatile std::uint8_t*>(p);while(n--)*q++=0;}
std::vector<std::uint8_t> sha256(const std::vector<std::uint8_t>&d){std::vector<std::uint8_t>o(SHA256_DIGEST_LENGTH);SHA256(d.data(),d.size(),o.data());return o;}
bool constantTimeEqual(const std::vector<std::uint8_t>&a,const std::vector<std::uint8_t>&b)noexcept{if(a.size()!=b.size())return false;std::uint8_t x=0;for(size_t i=0;i<a.size();++i)x|=a[i]^b[i];return x==0;}
void Credential::clear()noexcept{secureZero(secret.data(),secret.size());secret.clear();secret.shrink_to_fit();}
}