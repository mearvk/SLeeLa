#include "help_desk.hpp"
#include <algorithm>
#include <utility>

namespace sleela::philosophy {

HelpDesk::HelpDesk(std::size_t capacity, std::size_t company_size)
    : desk_(sl_help_desk_create(capacity, company_size)) {}

HelpDesk::~HelpDesk() { if (desk_) sl_help_desk_destroy(desk_); }

HelpDesk::HelpDesk(HelpDesk &&other) noexcept : desk_(other.desk_) {
    other.desk_ = nullptr;
}
HelpDesk &HelpDesk::operator=(HelpDesk &&other) noexcept {
    if (this != &other) {
        if (desk_) sl_help_desk_destroy(desk_);
        desk_ = other.desk_;
        other.desk_ = nullptr;
    }
    return *this;
}

sl_help_status HelpDesk::addContact(const sl_contact &c) noexcept {
    return desk_ ? sl_help_desk_add_contact(desk_, &c) : SL_HELP_INVALID_ARGUMENT;
}
std::size_t HelpDesk::seedPlaceholders(std::size_t count, std::uint64_t seed) noexcept {
    return desk_ ? sl_help_desk_seed_placeholders(desk_, count, seed) : 0u;
}
std::size_t HelpDesk::count() const noexcept { return sl_help_desk_count(desk_); }
std::size_t HelpDesk::capacity() const noexcept { return sl_help_desk_capacity(desk_); }
std::size_t HelpDesk::companySize() const noexcept { return sl_help_desk_company_size(desk_); }

bool HelpDesk::reachable(const sl_contact &) const noexcept { return true; }

std::vector<Respondent> HelpDesk::collect(
    const std::string &question, int role_filter, std::size_t max_results) const {
    std::vector<Respondent> result;
    if (!desk_ || question.empty()) return result;

    /* Pull more than requested so policy filtering still fills the window. */
    std::size_t window = (max_results == 0u) ? sl_help_desk_count(desk_)
                                             : max_results * 4u + 16u;
    std::vector<sl_respondent> raw(window);
    std::size_t total = 0u;
    std::size_t n = (role_filter < 0)
        ? sl_help_desk_query(desk_, question.c_str(), raw.data(), raw.size(), &total)
        : sl_help_desk_query_role(desk_, question.c_str(),
                                  static_cast<sl_help_role>(role_filter),
                                  raw.data(), raw.size(), &total);
    for (std::size_t i = 0u; i < n && result.size() < max_results; ++i) {
        const sl_contact *c = raw[i].contact;
        if (!c || !reachable(*c)) continue;
        Respondent r;
        r.contact = c;
        r.match_score = raw[i].match_score;
        r.alive = c->alive;
        r.known_of_science = c->known_of_science;
        r.role = sl_help_role_string(c->role);
        result.push_back(std::move(r));
    }
    return result;
}

std::vector<Respondent> HelpDesk::ask(const std::string &q, std::size_t max_results) const {
    return collect(q, -1, max_results);
}
std::vector<Respondent> HelpDesk::askRole(
    const std::string &q, sl_help_role role, std::size_t max_results) const {
    return collect(q, static_cast<int>(role), max_results);
}

bool ScienceHelpDesk::reachable(const sl_contact &c) const noexcept {
    return c.alive && c.known_of_science;
}

} // namespace sleela::philosophy
