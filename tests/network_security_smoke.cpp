#include "../api/native/network/sleela_network.hpp"
#include "../api/native/security/sleela_security.hpp"
#include <cassert>
int main(){using namespace sleela;std::vector<std::uint8_t>d{'s','l','e','e','l','a'};auto h=security::sha256(d);assert(h.size()==32);assert(security::constantTimeEqual(h,h));security::Credential c{"test",{1,2,3}};c.clear();assert(c.secret.empty());network::Socket s;assert(s.openState()==false);return 0;}