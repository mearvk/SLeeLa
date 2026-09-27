#ifndef HTTP80_CRYPTO_HPP
#define HTTP80_CRYPTO_HPP
#include "http80_crypto.h"
#include <string>
#include <vector>
namespace http80 { struct CryptoProfile { std::string key_agreement,kem,kdf,aead,signature; bool valid() const; }; std::vector<std::string> supported_crypto_names(); bool select_crypto(const std::string&,http80_crypto_selection&); }
#endif
