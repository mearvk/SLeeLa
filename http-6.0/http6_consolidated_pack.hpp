#ifndef SLEELA_HTTP6_CONSOLIDATED_PACK_HPP
#define SLEELA_HTTP6_CONSOLIDATED_PACK_HPP
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::http6 {
struct FriendRecord { std::string name; std::uint32_t points{0}; std::string document_reference; };
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
 void addFriend(std::string name,std::uint32_t points=0,std::string document_reference={});
 void setConsolidateIQ(std::uint64_t value) noexcept;
 void setTeamArea(std::string value);
 void setDebateTopic(std::string value);
 void setDebatePosition(std::string value);
 void setRecipientLabel(std::string value);
 const ConsolidatedPack& pack() const noexcept;
 std::string serialize() const;
};
}
#endif
