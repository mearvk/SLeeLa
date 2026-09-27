#ifndef SLEELA_HTTP5_FRIENDS_PACK_HPP
#define SLEELA_HTTP5_FRIENDS_PACK_HPP
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::http5 { struct BonusOffer { std::string reference; std::string description; }; class FriendsPack { public: explicit FriendsPack(std::uint32_t fp=0); std::uint32_t remaining()const noexcept; bool canOffer()const noexcept; bool consume()noexcept; void addOffer(BonusOffer offer); const std::vector<BonusOffer>& offers()const noexcept; private: std::uint32_t fp_; std::vector<BonusOffer> offers_; }; }
#endif
