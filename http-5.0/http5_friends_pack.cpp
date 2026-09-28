#include "http5_friends_pack.hpp"

#include <fstream>
#include <limits>
#include <utility>

namespace sleela::http5 {

FriendsPack::FriendsPack(std::uint32_t fp) : fp_(fp) {}

std::uint32_t FriendsPack::remaining() const noexcept { return fp_; }

bool FriendsPack::canOffer() const noexcept { return fp_ != 0; }

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
    return friend_entry.name + "|" +
           std::to_string(friend_entry.points) + "|" +
           friend_entry.document_reference;
}

bool FriendsPack::saveFriends(const std::string& path) const {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) return false;

    for (const auto& entry : friends_) {
        out << entry.name << '\t'
            << entry.points << '\t'
            << entry.document_reference << '\n';
    }
    return static_cast<bool>(out);
}

bool FriendsPack::loadFriends(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;

    std::vector<FriendEntry> loaded;
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        const std::size_t first = line.find('\t');
        const std::size_t second =
            first == std::string::npos ? std::string::npos :
            line.find('\t', first + 1);

        if (first == std::string::npos || second == std::string::npos) {
            return false;
        }

        FriendEntry entry;
        entry.name = line.substr(0, first);
        const std::string point_text =
            line.substr(first + 1, second - first - 1);
        entry.document_reference = line.substr(second + 1);

        try {
            std::size_t consumed = 0;
            const unsigned long value =
                std::stoul(point_text, &consumed, 10);
            if (consumed != point_text.size() ||
                value > static_cast<unsigned long>(
                    std::numeric_limits<std::uint32_t>::max())) {
                return false;
            }
            entry.points = static_cast<std::uint32_t>(value);
        } catch (...) {
            return false;
        }

        loaded.push_back(std::move(entry));
    }

    if (!in.eof() && in.fail()) return false;
    friends_ = std::move(loaded);
    return true;
}

bool FriendsPack::appendFriendToFile(const std::string& path,
                                      const FriendEntry& entry) {
    std::ofstream out(path, std::ios::binary | std::ios::app);
    if (!out) return false;

    out << entry.name << '\t'
        << entry.points << '\t'
        << entry.document_reference << '\n';
    return static_cast<bool>(out);
}

} // namespace sleela::http5
