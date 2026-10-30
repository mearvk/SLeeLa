#include "help_desk.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/*
 * Reach-out demo. Seeds 500 synthetic placeholder contacts for a ~1000-person
 * company, then answers a user Question/Query by listing the respondents with
 * their alive / known-of-Science status. Reads a question from argv or stdin;
 * falls back to a built-in query so it runs non-interactively (e.g. in CI).
 */
static void list_respondents(const sl_help_desk *desk, const char *question) {
    sl_respondent out[20];
    size_t total = 0u;
    size_t n = sl_help_desk_query(desk, question, out, 20u, &total);
    size_t i;
    printf("\nQuery: \"%s\"\n", question);
    printf("Respondents: %zu matched (showing %zu)\n", total, n);
    printf("%-28s %-26s %-10s %-22s %s\n",
           "name", "role", "status", "science", "reach");
    for (i = 0u; i < n; ++i) {
        const sl_contact *c = out[i].contact;
        printf("%-28s %-26s %-10s %-22s %s | %s | %s:%u\n",
               c->name, sl_help_role_string(c->role),
               sl_help_alive_string(c->alive),
               sl_help_science_string(c->known_of_science),
               c->email, c->phone, c->remote_ip, c->remote_port);
    }
}

int main(int argc, char **argv) {
    sl_help_desk *desk = sl_help_desk_create(512u, 1000u);
    char line[256];
    size_t seeded;
    if (!desk) return 1;

    seeded = sl_help_desk_seed_placeholders(desk, 500u, 1u);
    printf("Help / reach-out desk: %zu contacts (placeholder) for a company of %zu.\n",
           sl_help_desk_count(desk), sl_help_desk_company_size(desk));
    printf("NOTE: all contacts are synthetic RFC-safe placeholders; no real "
           "person is contacted.\n");
    (void)seeded;

    if (argc >= 2) {
        /* join argv into one question */
        char q[256]; int i; size_t pos = 0u;
        for (i = 1; i < argc && pos + 1u < sizeof(q); ++i)
            pos += (size_t)snprintf(q + pos, sizeof(q) - pos, "%s%s",
                                    i > 1 ? " " : "", argv[i]);
        list_respondents(desk, q);
    } else if (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\r\n")] = '\0';
        list_respondents(desk, line[0] ? line : "media interview mayor");
    } else {
        /* non-interactive fallback query */
        list_respondents(desk, "media interview broadcast");
        list_respondents(desk, "research science methods");
    }

    sl_help_desk_destroy(desk);
    return 0;
}
