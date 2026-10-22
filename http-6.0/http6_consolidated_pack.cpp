#include "http6_consolidated_pack.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace sleela::http6 {
namespace {
bool valid_utf8(const std::string& s) noexcept {
    const auto* p = reinterpret_cast<const unsigned char*>(s.data());
    std::size_t i = 0;
    while (i < s.size()) {
        const unsigned char c = p[i++];
        if (c <= 0x7f) continue;
        unsigned need = 0;
        std::uint32_t cp = 0;
        if (c >= 0xc2 && c <= 0xdf) { need = 1; cp = c & 0x1f; }
        else if (c >= 0xe0 && c <= 0xef) { need = 2; cp = c & 0x0f; }
        else if (c >= 0xf0 && c <= 0xf4) { need = 3; cp = c & 0x07; }
        else return false;
        if (i + need > s.size()) return false;
        for (unsigned j = 0; j < need; ++j) {
            const unsigned char d = p[i++];
            if ((d & 0xc0) != 0x80) return false;
            cp = (cp << 6) | (d & 0x3f);
        }
        if ((need == 1 && cp < 0x80) ||
            (need == 2 && cp < 0x800) ||
            (need == 3 && cp < 0x10000) ||
            (cp >= 0xd800 && cp <= 0xdfff) || cp > 0x10ffff) return false;
    }
    return true;
}

void check_string(const std::string& value, std::size_t max_bytes,
                  const char* field, bool allow_empty = true) {
    if ((!allow_empty && value.empty()) || value.size() > max_bytes)
        throw std::invalid_argument(std::string("HTTP/6.0 field out of bounds: ") + field);
    if (!valid_utf8(value))
        throw std::invalid_argument(std::string("HTTP/6.0 field is not valid UTF-8: ") + field);
}

void json_string(std::ostringstream& out, const std::string& value) {
    static const char hex[] = "0123456789abcdef";
    out << '"';
    for (unsigned char c : value) {
        switch (c) {
        case '"': out << "\\\""; break;
        case '\\': out << "\\\\"; break;
        case '\b': out << "\\b"; break;
        case '\f': out << "\\f"; break;
        case '\n': out << "\\n"; break;
        case '\r': out << "\\r"; break;
        case '\t': out << "\\t"; break;
        default:
            if (c < 0x20) { out << "\\u00" << hex[c >> 4] << hex[c & 0x0f]; }
            else out << static_cast<char>(c);
        }
    }
    out << '"';
}
} // namespace

void ConsolidatedFriendsBet::addFriend(std::string name, std::uint32_t points,
                                       std::string document_reference) {
    if (pack_.friends.size() >= MAX_FRIENDS)
        throw std::invalid_argument("HTTP/6.0 friend list exceeds maximum count");
    check_string(name, MAX_NAME_BYTES, "friend.name", false);
    check_string(document_reference, MAX_DOCUMENT_REFERENCE_BYTES, "friend.document_reference");
    pack_.friends.push_back(FriendRecord{std::move(name), points, std::move(document_reference)});
}

void ConsolidatedFriendsBet::setConsolidateIQ(std::uint64_t value) noexcept {
    pack_.consolidate_iq = value;
}
void ConsolidatedFriendsBet::setTeamArea(std::string value) {
    check_string(value, MAX_TEAM_AREA_BYTES, "team_area");
    pack_.team_area = std::move(value);
}
void ConsolidatedFriendsBet::setDebateTopic(std::string value) {
    check_string(value, MAX_DEBATE_TOPIC_BYTES, "debate_topic");
    pack_.debate_topic = std::move(value);
}
void ConsolidatedFriendsBet::setDebatePosition(std::string value) {
    check_string(value, MAX_DEBATE_POSITION_BYTES, "debate_position");
    pack_.debate_position = std::move(value);
}
void ConsolidatedFriendsBet::setRecipientLabel(std::string value) {
    check_string(value, MAX_RECIPIENT_LABEL_BYTES, "recipient_label");
    pack_.recipient_label = std::move(value);
}
const ConsolidatedPack& ConsolidatedFriendsBet::pack() const noexcept { return pack_; }

std::string ConsolidatedFriendsBet::serialize() const {
    std::ostringstream out;
    out << "{\"schema\":\"sleela-http6-consolidated-friends-bet-v1\",\"friends\":[";
    for (std::size_t i = 0; i < pack_.friends.size(); ++i) {
        if (i) out << ',';
        const auto& f = pack_.friends[i];
        /* Validate again in case a future API permits mutable pack access. */
        check_string(f.name, MAX_NAME_BYTES, "friend.name", false);
        check_string(f.document_reference, MAX_DOCUMENT_REFERENCE_BYTES, "friend.document_reference");
        out << "{\"name\":";
        json_string(out, f.name);
        out << ",\"points\":" << f.points << ",\"document_reference\":";
        json_string(out, f.document_reference);
        out << '}';
    }
    check_string(pack_.team_area, MAX_TEAM_AREA_BYTES, "team_area");
    check_string(pack_.debate_topic, MAX_DEBATE_TOPIC_BYTES, "debate_topic");
    check_string(pack_.debate_position, MAX_DEBATE_POSITION_BYTES, "debate_position");
    check_string(pack_.recipient_label, MAX_RECIPIENT_LABEL_BYTES, "recipient_label");
    out << "],\"consolidate_iq\":" << pack_.consolidate_iq << ",\"team_area\":";
    json_string(out, pack_.team_area);
    out << ",\"debate_topic\":";
    json_string(out, pack_.debate_topic);
    out << ",\"debate_position\":";
    json_string(out, pack_.debate_position);
    out << ",\"recipient_label\":";
    json_string(out, pack_.recipient_label);
    out << '}';
    return out.str();
}
} // namespace sleela::http6
