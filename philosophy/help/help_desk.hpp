#ifndef SLEELA_PHILOSOPHY_HELP_DESK_HPP
#define SLEELA_PHILOSOPHY_HELP_DESK_HPP
/*
 * C++17 wrapper over the C help desk. Inheritable: derive and override
 * reachable() to apply application policy about which contacts a given user is
 * permitted to reach (for example, hide deceased contacts, or restrict to
 * contacts known of Science). The base policy permits every matched contact.
 *
 * Ships no real contact data. See help_desk.h for the data boundary.
 */
#include "help_desk.h"
#include <cstdint>
#include <string>
#include <vector>

namespace sleela::philosophy {

struct Respondent {
    const sl_contact *contact = nullptr;
    unsigned match_score = 0u;
    bool alive = false;
    bool known_of_science = false;
    std::string role;        /* human-readable role */
};

class HelpDesk {
public:
    explicit HelpDesk(std::size_t capacity = SL_HELP_DEFAULT_CAPACITY,
                      std::size_t company_size = SL_HELP_DEFAULT_COMPANY_SIZE);
    virtual ~HelpDesk();
    HelpDesk(const HelpDesk &) = delete;
    HelpDesk &operator=(const HelpDesk &) = delete;
    HelpDesk(HelpDesk &&) noexcept;
    HelpDesk &operator=(HelpDesk &&) noexcept;

    sl_help_status addContact(const sl_contact &c) noexcept;
    std::size_t seedPlaceholders(std::size_t count, std::uint64_t seed = 1u) noexcept;

    std::size_t count() const noexcept;
    std::size_t capacity() const noexcept;
    std::size_t companySize() const noexcept;

    /* Submit a Question/Query; returns ranked respondents the policy allows,
     * each carrying alive / known-of-Science status. */
    std::vector<Respondent> ask(const std::string &question,
                                std::size_t max_results = 25u) const;
    std::vector<Respondent> askRole(const std::string &question, sl_help_role role,
                                    std::size_t max_results = 25u) const;

protected:
    /* Policy hook: return false to withhold a matched contact. Base: allow. */
    virtual bool reachable(const sl_contact &c) const noexcept;

private:
    std::vector<Respondent> collect(const std::string &question, int role_filter,
                                    std::size_t max_results) const;
    sl_help_desk *desk_;
};

/* A policy that only surfaces living respondents who are known of Science. */
class ScienceHelpDesk final : public HelpDesk {
public:
    using HelpDesk::HelpDesk;
protected:
    bool reachable(const sl_contact &c) const noexcept override;
};

} // namespace sleela::philosophy
#endif
