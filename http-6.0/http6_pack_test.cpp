#include "http6_consolidated_pack.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    using sleela::http6::ConsolidatedFriendsBet;
    ConsolidatedFriendsBet pack;
    pack.addFriend("Ada \"Lovelace\"", 42, "doc://example/one");
    pack.setConsolidateIQ(18446744073709551615ULL);
    pack.setTeamArea("Research");
    pack.setDebateTopic("Protocol \"safety\"");
    pack.setDebatePosition("Line one\nLine two");
    pack.setRecipientLabel("Review team");
    const std::string json = pack.serialize();
    assert(json.find("\\\"Lovelace\\\"") != std::string::npos);
    assert(json.find("Line one\\nLine two") != std::string::npos);
    assert(json.find("18446744073709551615") != std::string::npos);
    assert(pack.pack().friends.size() == 1);

    bool rejected = false;
    try { pack.addFriend("", 0, ""); }
    catch (const std::invalid_argument&) { rejected = true; }
    assert(rejected);

    rejected = false;
    try { pack.setTeamArea(std::string(ConsolidatedFriendsBet::MAX_TEAM_AREA_BYTES + 1, 'x')); }
    catch (const std::invalid_argument&) { rejected = true; }
    assert(rejected);

    rejected = false;
    try { pack.setDebateTopic(std::string("\\xff", 1)); }
    catch (const std::invalid_argument&) { rejected = true; }
    assert(rejected);

    ConsolidatedFriendsBet full;
    for (std::size_t i = 0; i < ConsolidatedFriendsBet::MAX_FRIENDS; ++i)
        full.addFriend("friend-" + std::to_string(i));
    rejected = false;
    try { full.addFriend("too-many"); }
    catch (const std::invalid_argument&) { rejected = true; }
    assert(rejected);

    std::cout << "http6_pack_test: all tests passed\\n";
    return 0;
}
