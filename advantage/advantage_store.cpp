// advantage_store.cpp - Implementation of the advantages.xml loader.
//
// A compact, dependency-free parser for the advantages.xml schema. It walks
// the document element-by-element, understanding exactly the tags this
// module emits: <advantages>, <sourceAdvantage>, <name>, <description>,
// <assumptions>/<assumption> (optional status="held|doubted">),
// <knowledge>/<string>, <knowledgeSource>, and <evidence>/<observation>.
// Unknown tags are skipped, but malformed values in the fields that feed the
// math (numbers, assumption status, duplicate ids, stray <observation>) are
// rejected with an error rather than silently coerced. This avoids pulling in an external XML library while still
// loading the data faithfully into memory for analysis.
#include "advantage_store.hpp"

#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>

namespace advantage {

const SourceAdvantage* AdvantageStore::find(const std::string& id) const {
    for (const auto& s : sources) {
        if (s.id == id) return &s;
    }
    return nullptr;
}

std::size_t AdvantageStore::assumptionCount() const {
    std::size_t n = 0;
    for (const auto& s : sources) n += s.assumptions.size();
    return n;
}

std::size_t AdvantageStore::knowledgeCount() const {
    std::size_t n = 0;
    for (const auto& s : sources) n += s.knowledge.size();
    return n;
}

std::size_t AdvantageStore::evidenceCount() const {
    std::size_t n = 0;
    for (const auto& s : sources) n += s.evidence.size();
    return n;
}

namespace {

// Minimal cursor-based scanner over the XML text.
struct Scanner {
    const std::string& s;
    std::size_t i = 0;
    explicit Scanner(const std::string& text) : s(text) {}

    bool eof() const { return i >= s.size(); }

    // Advance to the next '<'. Returns false at end of input.
    bool toNextTag() {
        while (i < s.size() && s[i] != '<') ++i;
        return i < s.size();
    }
};

// Replace the handful of XML entities our documents can contain.
std::string unescape(const std::string& in) {
    std::string out;
    out.reserve(in.size());
    for (std::size_t k = 0; k < in.size();) {
        if (in[k] == '&') {
            if (in.compare(k, 5, "&amp;") == 0) { out += '&'; k += 5; continue; }
            if (in.compare(k, 4, "&lt;") == 0)  { out += '<'; k += 4; continue; }
            if (in.compare(k, 4, "&gt;") == 0)  { out += '>'; k += 4; continue; }
            if (in.compare(k, 6, "&quot;") == 0){ out += '"'; k += 6; continue; }
            if (in.compare(k, 6, "&apos;") == 0){ out += '\''; k += 6; continue; }
        }
        out += in[k++];
    }
    return out;
}

std::string trim(const std::string& in) {
    std::size_t a = 0, b = in.size();
    while (a < b && std::isspace(static_cast<unsigned char>(in[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(in[b - 1]))) --b;
    return in.substr(a, b - a);
}

// Represents one parsed start/end/empty/decl/comment tag.
struct Tag {
    enum Type { Start, End, Empty, Skip } type = Skip;
    std::string name;
    std::string attrs;  // raw attribute text (between name and '>')
};

// Read the next tag starting at a '<'. Advances the scanner past '>'.
bool readTag(Scanner& sc, Tag& tag) {
    if (!sc.toNextTag()) return false;
    ++sc.i;  // consume '<'

    // Comments and declarations: skip wholesale.
    if (sc.i < sc.s.size() && (sc.s[sc.i] == '!' || sc.s[sc.i] == '?')) {
        if (sc.s.compare(sc.i, 3, "!--") == 0) {
            std::size_t end = sc.s.find("-->", sc.i);
            sc.i = (end == std::string::npos) ? sc.s.size() : end + 3;
        } else {
            std::size_t end = sc.s.find('>', sc.i);
            sc.i = (end == std::string::npos) ? sc.s.size() : end + 1;
        }
        tag.type = Tag::Skip;
        return true;
    }

    bool closing = false;
    if (sc.i < sc.s.size() && sc.s[sc.i] == '/') { closing = true; ++sc.i; }

    // Tag name.
    std::size_t start = sc.i;
    while (sc.i < sc.s.size() && sc.s[sc.i] != '>' && sc.s[sc.i] != '/' &&
           !std::isspace(static_cast<unsigned char>(sc.s[sc.i]))) {
        ++sc.i;
    }
    tag.name = sc.s.substr(start, sc.i - start);

    // Remainder up to '>' is attributes (and possible self-closing '/').
    std::size_t gt = sc.s.find('>', sc.i);
    if (gt == std::string::npos) return false;
    std::string rest = sc.s.substr(sc.i, gt - sc.i);
    sc.i = gt + 1;

    bool selfClose = (!rest.empty() && rest.back() == '/');
    if (selfClose) rest.pop_back();
    tag.attrs = rest;

    if (closing)       tag.type = Tag::End;
    else if (selfClose) tag.type = Tag::Empty;
    else               tag.type = Tag::Start;
    return true;
}

// Pull a single attribute value out of a raw attribute string.
std::string attr(const std::string& attrs, const std::string& key) {
    std::string needle = key + "=\"";
    std::size_t p = attrs.find(needle);
    if (p == std::string::npos) return "";
    p += needle.size();
    std::size_t q = attrs.find('"', p);
    if (q == std::string::npos) return "";
    return unescape(attrs.substr(p, q - p));
}

// Read text content from the current position up to the next '<'.
std::string readText(Scanner& sc) {
    std::size_t start = sc.i;
    while (sc.i < sc.s.size() && sc.s[sc.i] != '<') ++sc.i;
    return trim(unescape(sc.s.substr(start, sc.i - start)));
}

// Strict number parse: the whole string must be a finite number. Fails
// closed so a typo in the XML can never silently become 0.
bool parseNumber(const std::string& text, double& out) {
    if (text.empty()) return false;
    char* end = nullptr;
    errno = 0;
    double v = std::strtod(text.c_str(), &end);
    if (errno != 0 || end == text.c_str() || *end != '\0') return false;
    if (!std::isfinite(v)) return false;
    out = v;
    return true;
}

}  // namespace

bool loadFromString(const std::string& xml,
                    AdvantageStore& out,
                    std::string& error) {
    out = AdvantageStore{};
    Scanner sc(xml);
    Tag tag;

    SourceAdvantage* current = nullptr;
    bool inAssumptions = false;
    bool inKnowledge = false;
    bool inEvidence = false;

    while (readTag(sc, tag)) {
        if (tag.type == Tag::Skip) continue;

        if (tag.type == Tag::Start || tag.type == Tag::Empty) {
            if (tag.name == "advantages") {
                out.version = attr(tag.attrs, "version");
            } else if (tag.name == "sourceAdvantage") {
                out.sources.emplace_back();
                current = &out.sources.back();
                current->id = attr(tag.attrs, "id");
                if (!current->id.empty()) {
                    for (std::size_t k = 0; k + 1 < out.sources.size(); ++k) {
                        if (out.sources[k].id == current->id) {
                            error = "duplicate sourceAdvantage id '" +
                                    current->id + "'";
                            return false;
                        }
                    }
                }
                current->kind = attr(tag.attrs, "kind");
            } else if (tag.name == "knowledgeSource") {
                // Declared remote origin. Recorded only; never fetched here.
                if (current) {
                    KnowledgeSource ks;
                    ks.url = attr(tag.attrs, "url");
                    ks.sha256 = attr(tag.attrs, "sha256");
                    if (!ks.url.empty()) current->knowledgeSources.push_back(ks);
                }
            } else if (tag.name == "assumptions") {
                inAssumptions = true;
            } else if (tag.name == "knowledge") {
                inKnowledge = true;
            } else if (tag.name == "evidence" && current) {
                inEvidence = tag.type == Tag::Start;
                std::string w = attr(tag.attrs, "weight");
                if (!w.empty() && !parseNumber(w, current->evidenceWeight)) {
                    error = "source '" + current->id +
                            "': invalid evidence weight '" + w + "'";
                    return false;
                }
                if (current->evidenceWeight < 0.0) {
                    error = "source '" + current->id +
                            "': evidence weight must not be negative";
                    return false;
                }
            } else if (tag.name == "observation" && current && inEvidence) {
                Observation ob;
                if (!parseNumber(attr(tag.attrs, "reward"), ob.reward) ||
                    !parseNumber(attr(tag.attrs, "baseline"), ob.baseline)) {
                    error = "source '" + current->id +
                            "': <observation> needs numeric reward and baseline";
                    return false;
                }
                current->evidence.push_back(ob);
            } else if (tag.name == "observation") {
                error = "<observation> must appear inside <evidence> within a "
                        "<sourceAdvantage>";
                return false;
            } else if (current && tag.type == Tag::Start) {
                if (tag.name == "name") {
                    current->name = readText(sc);
                } else if (tag.name == "description") {
                    current->description = readText(sc);
                } else if (tag.name == "assumption" && inAssumptions) {
                    const std::string status = attr(tag.attrs, "status");
                    if (!status.empty() && status != "held" &&
                        status != "doubted") {
                        error = "source '" + current->id +
                                "': assumption status must be 'held' or "
                                "'doubted', got '" + status + "'";
                        return false;
                    }
                    current->assumptions.push_back(readText(sc));
                    current->assumptionHeld.push_back(status != "doubted");
                } else if (tag.name == "string" && inKnowledge) {
                    current->knowledge.push_back(readText(sc));
                }
            }
        } else if (tag.type == Tag::End) {
            if (tag.name == "assumptions") inAssumptions = false;
            else if (tag.name == "knowledge") inKnowledge = false;
            else if (tag.name == "evidence") inEvidence = false;
            else if (tag.name == "sourceAdvantage") current = nullptr;
        }
    }

    if (out.sources.empty()) {
        error = "no <sourceAdvantage> elements found";
        return false;
    }
    error.clear();
    return true;
}

bool loadFromFile(const std::string& path,
                  AdvantageStore& out,
                  std::string& error) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        error = "could not open file: " + path;
        return false;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return loadFromString(ss.str(), out, error);
}

}  // namespace advantage
