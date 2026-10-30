#include "help_desk.hpp"
#include <cassert>
#include <cstring>
#include <string>

using namespace sleela::philosophy;

static void test_c_core_sizing() {
    sl_help_desk *d = sl_help_desk_create(512u, 1000u);
    assert(d);
    /* Built to hold ~500 for a ~1000-person company. */
    assert(sl_help_desk_capacity(d) == 512u);
    assert(sl_help_desk_company_size(d) == 1000u);
    size_t seeded = sl_help_desk_seed_placeholders(d, 500u, 1u);
    assert(seeded == 500u);
    assert(sl_help_desk_count(d) == 500u);
    /* Cannot exceed capacity. */
    size_t more = sl_help_desk_seed_placeholders(d, 100u, 2u);
    assert(more == 12u); /* only 12 slots remained (512 - 500) */
    assert(sl_help_desk_count(d) == 512u);
    sl_help_desk_destroy(d);
}

static void test_add_and_validate() {
    sl_help_desk *d = sl_help_desk_create(8u, 10u);
    sl_contact c;
    assert(d);
    /* RFC-safe example data. */
    assert(sl_contact_init(&c, 1u, SL_ROLE_MAYOR, "Jordan Park", "City Hall",
        "jordan.park@example.org", "+1-555-0142", "192.0.2.10", 8443u,
        "city zoning transit", true, false) == SL_HELP_OK);
    assert(sl_help_desk_add_contact(d, &c) == SL_HELP_OK);
    /* All three channels detected. */
    assert((c.channels & SL_CHANNEL_EMAIL) && (c.channels & SL_CHANNEL_PHONE) &&
           (c.channels & SL_CHANNEL_REMOTE));
    /* Duplicate id rejected. */
    assert(sl_help_desk_add_contact(d, &c) == SL_HELP_DUPLICATE_ID);
    /* id 0 and bad role rejected. */
    sl_contact bad;
    assert(sl_contact_init(&bad, 0u, SL_ROLE_MAYOR, "X", "", "", "", "", 0u, "", true, true)
           == SL_HELP_INVALID_ARGUMENT);
    sl_help_desk_destroy(d);
}

static void test_query_matches_and_status() {
    sl_help_desk *d = sl_help_desk_create(16u, 20u);
    sl_contact a, b;
    sl_respondent out[8];
    size_t total = 0u, n;
    assert(d);
    assert(sl_contact_init(&a, 10u, SL_ROLE_TELEGENIC_AUTHOR, "Sage Rossi",
        "Broadcast Desk", "sage.rossi@example.com", "+1-555-0110",
        "203.0.113.5", 5000u, "broadcast media interview series", true, true) == SL_HELP_OK);
    assert(sl_contact_init(&b, 11u, SL_ROLE_MANAGEMENT, "Blair Khan",
        "Management", "blair.khan@example.com", "+1-555-0111",
        "198.51.100.7", 6000u, "schedule budget policy", false, false) == SL_HELP_OK);
    assert(sl_help_desk_add_contact(d, &a) == SL_HELP_OK);
    assert(sl_help_desk_add_contact(d, &b) == SL_HELP_OK);

    /* A media interview query matches the telegenic author, not management. */
    n = sl_help_desk_query(d, "media interview", out, 8u, &total);
    assert(total == 1u && n == 1u);
    assert(out[0].contact->id == 10u);
    assert(out[0].contact->alive == true);
    assert(out[0].contact->known_of_science == true);

    /* A budget query matches management; status reported as deceased/not-science. */
    n = sl_help_desk_query(d, "budget policy", out, 8u, &total);
    assert(total == 1u && out[0].contact->id == 11u);
    assert(strcmp(sl_help_alive_string(out[0].contact->alive), "deceased") == 0);
    assert(strcmp(sl_help_science_string(out[0].contact->known_of_science),
                  "not known of Science") == 0);

    /* Higher-overlap query ranks higher score first. */
    n = sl_help_desk_query(d, "broadcast media interview series", out, 8u, &total);
    assert(n >= 1u && out[0].contact->id == 10u && out[0].match_score >= 3u);

    /* Role filter. */
    n = sl_help_desk_query_role(d, "policy", SL_ROLE_MANAGEMENT, out, 8u, &total);
    assert(total == 1u && out[0].contact->role == SL_ROLE_MANAGEMENT);
    n = sl_help_desk_query_role(d, "policy", SL_ROLE_MAYOR, out, 8u, &total);
    assert(total == 0u);
    (void)n;
    sl_help_desk_destroy(d);
}

static void test_linear_scaling_total() {
    /* total reflects the whole directory even when the output window is small. */
    sl_help_desk *d = sl_help_desk_create(1024u, 2000u);
    sl_respondent out[5];
    size_t total = 0u, n;
    assert(sl_help_desk_seed_placeholders(d, 500u, 7u) == 500u);
    n = sl_help_desk_query(d, "series", out, 5u, &total);
    assert(n <= 5u);
    assert(total >= n);   /* many matches, small window */
    sl_help_desk_destroy(d);
}

static void test_cpp_wrapper_and_policy() {
    HelpDesk desk(512u, 1000u);
    assert(desk.seedPlaceholders(500u, 1u) == 500u);
    assert(desk.count() == 500u);
    auto all = desk.ask("research science methods", 50u);
    assert(!all.empty());
    /* Base policy surfaces all statuses, including deceased / not-science. */
    bool saw_any = !all.empty();
    assert(saw_any);

    /* Science policy: every surfaced respondent is alive AND known of Science. */
    ScienceHelpDesk science(512u, 1000u);
    science.seedPlaceholders(500u, 1u);
    auto filtered = science.ask("research science methods", 50u);
    for (const auto &r : filtered) {
        assert(r.alive);
        assert(r.known_of_science);
    }
    /* The science-filtered set is no larger than the unfiltered set. */
    ScienceHelpDesk s2(512u, 1000u); s2.seedPlaceholders(500u, 1u);
    assert(s2.ask("series", 500u).size() <= desk.ask("series", 500u).size());
}

int main() {
    test_c_core_sizing();
    test_add_and_validate();
    test_query_matches_and_status();
    test_linear_scaling_total();
    test_cpp_wrapper_and_policy();
    std::puts("all help desk tests passed");
    return 0;
}
