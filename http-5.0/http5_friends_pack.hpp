#ifndef SLEELA_HTTP5_FRIENDS_PACK_HPP
#define SLEELA_HTTP5_FRIENDS_PACK_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace sleela::http5 {

struct BonusOffer {
    std::string reference;
    std::string description;
};

struct FriendEntry {
    std::string name;
    std::uint32_t points{0};
    std::string document_reference;
};

class FriendsPack {
public:
    explicit FriendsPack(std::uint32_t fp=0);

    std::uint32_t remaining() const noexcept;
    bool canOffer() const noexcept;
    bool consume() noexcept;

    void addOffer(BonusOffer offer);
    const std::vector<BonusOffer>& offers() const noexcept;

    // Simple application-managed friend list.
    void addFriend(std::string name, std::uint32_t points=0,
                   std::string document_reference={});
    const std::vector<FriendEntry>& friends() const noexcept;

    // Builds a bounded, ordinary application payload.
    std::string friendPayload(std::size_t index) const;

    // File I/O for the user-managed friends list.
    // The file is application data and is never treated as an executable command.
    bool saveFriends(const std::string& path) const;
    bool loadFriends(const std::string& path);
    static bool appendFriendToFile(const std::string& path,
                                   const FriendEntry& entry);

private:
    std::uint32_t fp_;
    std::vector<BonusOffer> offers_;
    std::vector<FriendEntry> friends_;
};

} // namespace sleela::http5

#endif
