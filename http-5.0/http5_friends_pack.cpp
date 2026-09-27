#include "http5_friends_pack.hpp"

#include <utility>

namespace sleela::http5 {

FriendsPack::FriendsPack(std::uint32_t fp) : fp_(fp) {}

std::uint32_t FriendsPack::remaining() const noexcept {
    return fp_;
}

bool FriendsPack::canOffer() const noexcept {
    return fp_ != 0;
}

bool FriendsPack::consume() noexcept {
    if (!fp_) return false;
    --fp_;
    return true;
}

void FriendsPack::addOffer(BonusOffer offer) {
    offers_.push_back(std::move(offer));
}

const std::vector<BonusOffer>& FriendsPack::offers() const noexcept {
    return offers_;
}

void FriendsPack::addFriend(std::string name, std::uint32_t points,
                            std::string document_reference) {
    friends_.push_back(FriendEntry{
        std::move(name), points, std::move(document_reference)
    });
}

const std::vector<FriendEntry>& FriendsPack::friends() const noexcept {
    return friends_;
}

std::string FriendsPack::friendPayload(std::size_t index) const {
    if (index >= friends_.size()) return {};

    const FriendEntry& friend_entry = friends_[index];

    // Deliberately simple, inspectable application payload:
    // friend name + point amount + assigned document reference.
    // No command, routing instruction, or infrastructure-control field is
    // generated here.
    return friend_entry.name + "|" +
           std::to_string(friend_entry.points) + "|" +
           friend_entry.document_reference;
}

} // namespace sleela::http5
