#ifndef SLEELA_HTTP6_CONSOLIDATED_PACK_HPP
#define SLEELA_HTTP6_CONSOLIDATED_PACK_HPP
#include <cstdint>
#include <string>
#include <vector>

namespace sleela::http6 {
struct FriendRecord {
    std::string name;
    std::uint32_t points{0};
    std::string document_reference;
};
struct ConsolidatedPack {
    std::vector<FriendRecord> friends;
    std::uint64_t consolidate_iq{0};
    std::string team_area;
    std::string debate_topic;
    std::string debate_position;
    std::string recipient_label;
};

class ConsolidatedFriendsBet {
public:
    static constexpr std::size_t MAX_FRIENDS = 256;
    static constexpr std::size_t MAX_NAME_BYTES = 128;
    static constexpr std::size_t MAX_DOCUMENT_REFERENCE_BYTES = 512;
    static constexpr std::size_t MAX_TEAM_AREA_BYTES = 128;
    static constexpr std::size_t MAX_DEBATE_TOPIC_BYTES = 1024;
    static constexpr std::size_t MAX_DEBATE_POSITION_BYTES = 8192;
    static constexpr std::size_t MAX_RECIPIENT_LABEL_BYTES = 256;

    void addFriend(std::string name, std::uint32_t points = 0,
                   std::string document_reference = {});
    void setConsolidateIQ(std::uint64_t value) noexcept;
    void setTeamArea(std::string value);
    void setDebateTopic(std::string value);
    void setDebatePosition(std::string value);
    void setRecipientLabel(std::string value);
    const ConsolidatedPack& pack() const noexcept;
    /* Deterministic UTF-8 JSON. Throws std::invalid_argument for invalid UTF-8
       or fields outside the documented byte bounds. */
    std::string serialize() const;

private:
    ConsolidatedPack pack_;
};
} // namespace sleela::http6
#endif
