#include "help_desk.hpp"
#include <cstdio>
#include <string>

using namespace sleela::philosophy;

static void show(const char *title, const std::vector<Respondent> &rs) {
    std::printf("\n%s -- %zu respondent(s)\n", title, rs.size());
    for (const auto &r : rs) {
        std::printf("  %-26s %-26s %-9s | %s\n",
                    r.contact->name, r.role.c_str(),
                    r.alive ? "alive" : "deceased",
                    r.known_of_science ? "known of Science" : "not known of Science");
    }
}

int main() {
    /* Base desk: lists every matched respondent. */
    HelpDesk desk(512u, 1000u);
    desk.seedPlaceholders(500u, 1u);
    std::printf("C++ reach-out desk: %zu placeholder contacts for a company of %zu.\n",
                desk.count(), desk.companySize());

    show("ask: \"city transit policy\"", desk.ask("city transit policy", 8u));
    show("askRole: mayors on \"public statement\"",
         desk.askRole("public statement press", SL_ROLE_MAYOR, 8u));

    /* Policy subclass: only living respondents known of Science. */
    ScienceHelpDesk science(512u, 1000u);
    science.seedPlaceholders(500u, 1u);
    show("ScienceHelpDesk: \"research science methods\" (alive + known of Science only)",
         science.ask("research science methods", 8u));
    return 0;
}
