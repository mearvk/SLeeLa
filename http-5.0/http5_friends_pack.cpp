#include "http5_friends_pack.hpp"
#include <utility>
namespace sleela::http5 { FriendsPack::FriendsPack(std::uint32_t fp):fp_(fp){} std::uint32_t FriendsPack::remaining()const noexcept{return fp_;} bool FriendsPack::canOffer()const noexcept{return fp_!=0;} bool FriendsPack::consume()noexcept{if(!fp_)return false;--fp_;return true;} void FriendsPack::addOffer(BonusOffer offer){offers_.push_back(std::move(offer));} const std::vector<BonusOffer>& FriendsPack::offers()const noexcept{return offers_;} }
