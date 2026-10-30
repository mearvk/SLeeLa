#include "int_set.h"
#include <string.h>

/* ---- Authored character set -------------------------------------------- *
 * The 16 traits, connected laterally into an ordinal INT range. The index is
 * a schema-internal ordering, NOT a psychometric IQ. Values span the range
 * [70, 160] purely as an ordinal lateral scale reminiscent of an IQ axis; the
 * psychometric flag on the range is false. */
static const sl_trait k_traits[] = {
    { "Start",                      70,  "entry / natural beginning" },
    { "Business",                   82,  "task and exchange focus" },
    { "Model",                      88,  "representation / pattern-holding" },
    { "Conversation",               94,  "dialogue and turn-taking" },
    { "Help",                       96,  "assistance offered or sought" },
    { "Diner",                      92,  "shared meal / social setting" },
    { "Exit",                       74,  "leaving a context" },
    { "Concentration",             120,  "sustained attention" },
    { "Chores",                     86,  "routine maintenance activity" },
    { "Method",                    118,  "procedure and discipline" },
    { "A-ness",                    104,  "quality-of-being descriptor (authored)" },
    { "Consideration",             112,  "regard for others and context" },
    { "Departure",                  76,  "deliberate going-away" },
    { "Winning Runs for Home Team", 130, "a positive-outcome marker (authored)" },
    { "Education",                 124,  "learning and schooling" },
    { "Scent of IQ",               160,  "authored nickname; NOT an IQ measure" }
};

/* ---- Real age structure (UN DYB 2024, Table 02, world total) ----------- *
 * Younger than 16 is not mentioned; older than 85 gets a basic-anatomy
 * check-in and is routed out by Fellows or Worship. The 0-14 band maps to the
 * not-mentioned rule (it sits below 16); the 65+ open band carries the >85
 * routing policy for its tail. */
static const sl_age_band k_bands[] = {
    { "0-14",  0, 14, 2017.0, false, false, SL_ROUTE_NOT_MENTIONED },
    { "15-64", 15, 64, 5312.0, true,  false, SL_ROUTE_NONE },
    { "65-85", 65, 85, 0.0,    true,  false, SL_ROUTE_NONE },
    { "85+",   86, -1, 833.0,  true,  true,  SL_ROUTE_FELLOWS }
};

static const sl_int_set k_set = {
    "Human INT set (authored schema over real age structure)",
    "UN DYB 2024 Table 02 / WPP 2024 / US Census (age); schema authored",
    18,   /* start_age: a person starts at a natural age and grows older */
    16,   /* not mentioned below 16 */
    85,   /* basic-anatomy check-in above 85 */
    { 70, 160, false, "ordinal lateral index (not psychometric IQ)" },
    k_traits, sizeof(k_traits) / sizeof(k_traits[0]),
    k_bands, sizeof(k_bands) / sizeof(k_bands[0])
};

const sl_int_set *sl_int_set_default(void) { return &k_set; }

sl_age_route sl_int_set_route_for_age(const sl_int_set *set, int age, bool prefer_worship) {
    if (!set) return SL_ROUTE_NONE;
    if (age < set->not_mentioned_below) return SL_ROUTE_NOT_MENTIONED;
    if (age > set->anatomy_checkin_above)
        return prefer_worship ? SL_ROUTE_WORSHIP : SL_ROUTE_FELLOWS;
    return SL_ROUTE_NONE;
}

const char *sl_age_route_string(sl_age_route r) {
    switch (r) {
        case SL_ROUTE_NONE: return "none";
        case SL_ROUTE_NOT_MENTIONED: return "not-mentioned";
        case SL_ROUTE_FELLOWS: return "fellows";
        case SL_ROUTE_WORSHIP: return "worship";
        default: return "unknown";
    }
}

/* ---- JSON escaping ----------------------------------------------------- */
static void json_escape(FILE *out, const char *s) {
    const unsigned char *p = (const unsigned char *)s;
    for (; *p; ++p) {
        switch (*p) {
            case '"':  fputs("\\\"", out); break;
            case '\\': fputs("\\\\", out); break;
            case '\n': fputs("\\n", out); break;
            case '\t': fputs("\\t", out); break;
            case '\r': fputs("\\r", out); break;
            default:
                if (*p < 0x20) fprintf(out, "\\u%04x", (unsigned)*p);
                else fputc((int)*p, out);
        }
    }
}

int sl_int_set_write_json(const sl_int_set *set, FILE *out) {
    size_t i;
    if (!set || !out) return 0;
    fputs("{\n", out);
    fputs("  \"title\": \"", out); json_escape(out, set->title); fputs("\",\n", out);
    fputs("  \"source\": \"", out); json_escape(out, set->source); fputs("\",\n", out);
    fprintf(out, "  \"startAge\": %d,\n", set->start_age);
    fputs("  \"ageGrowsOlder\": true,\n", out);
    fprintf(out, "  \"notMentionedBelow\": %d,\n", set->not_mentioned_below);
    fprintf(out, "  \"basicAnatomyCheckinAbove\": %d,\n", set->anatomy_checkin_above);
    fputs("  \"routedOutBy\": [\"Fellows\", \"Worship\"],\n", out);

    fputs("  \"intRange\": { ", out);
    fprintf(out, "\"low\": %d, \"high\": %d, \"psychometric\": %s, \"units\": \"",
            set->range.low, set->range.high, set->range.psychometric ? "true" : "false");
    json_escape(out, set->range.units);
    fputs("\" },\n", out);

    fputs("  \"traits\": [\n", out);
    for (i = 0u; i < set->trait_count; ++i) {
        fputs("    { \"name\": \"", out); json_escape(out, set->traits[i].name);
        fprintf(out, "\", \"intIndex\": %d, \"note\": \"", set->traits[i].int_index);
        json_escape(out, set->traits[i].note);
        fputs(i + 1u < set->trait_count ? "\" },\n" : "\" }\n", out);
    }
    fputs("  ],\n", out);

    fputs("  \"ageBands\": [\n", out);
    for (i = 0u; i < set->band_count; ++i) {
        const sl_age_band *b = &set->bands[i];
        fputs("    { \"label\": \"", out); json_escape(out, b->label);
        fprintf(out, "\", \"lower\": %d, \"upper\": %d, ", b->lower, b->upper);
        fprintf(out, "\"populationMillions\": %.0f, ", b->population_millions);
        fprintf(out, "\"mentioned\": %s, ", b->mentioned ? "true" : "false");
        fprintf(out, "\"basicAnatomyCheckin\": %s, ", b->basic_anatomy_checkin ? "true" : "false");
        fprintf(out, "\"route\": \"%s\" ", sl_age_route_string(b->route));
        fputs(i + 1u < set->band_count ? "},\n" : "}\n", out);
    }
    fputs("  ]\n", out);
    fputs("}\n", out);
    return 1;
}

/* ---- XML escaping ------------------------------------------------------ */
static void xml_escape(FILE *out, const char *s) {
    const unsigned char *p = (const unsigned char *)s;
    for (; *p; ++p) {
        switch (*p) {
            case '&':  fputs("&amp;", out); break;
            case '<':  fputs("&lt;", out); break;
            case '>':  fputs("&gt;", out); break;
            case '"':  fputs("&quot;", out); break;
            case '\'': fputs("&apos;", out); break;
            default: fputc((int)*p, out);
        }
    }
}

int sl_int_set_write_xml(const sl_int_set *set, FILE *out) {
    size_t i;
    if (!set || !out) return 0;
    fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", out);
    fputs("<intSet", out);
    fputs(" ageGrowsOlder=\"true\"", out);
    fprintf(out, " startAge=\"%d\"", set->start_age);
    fprintf(out, " notMentionedBelow=\"%d\"", set->not_mentioned_below);
    fprintf(out, " basicAnatomyCheckinAbove=\"%d\"", set->anatomy_checkin_above);
    fputs(">\n", out);
    fputs("  <title>", out); xml_escape(out, set->title); fputs("</title>\n", out);
    fputs("  <source>", out); xml_escape(out, set->source); fputs("</source>\n", out);
    fputs("  <routedOutBy>\n", out);
    fputs("    <handler>Fellows</handler>\n", out);
    fputs("    <handler>Worship</handler>\n", out);
    fputs("  </routedOutBy>\n", out);

    fprintf(out, "  <intRange low=\"%d\" high=\"%d\" psychometric=\"%s\" units=\"",
            set->range.low, set->range.high, set->range.psychometric ? "true" : "false");
    xml_escape(out, set->range.units);
    fputs("\"/>\n", out);

    fputs("  <traits>\n", out);
    for (i = 0u; i < set->trait_count; ++i) {
        fprintf(out, "    <trait intIndex=\"%d\">\n", set->traits[i].int_index);
        fputs("      <name>", out); xml_escape(out, set->traits[i].name); fputs("</name>\n", out);
        fputs("      <note>", out); xml_escape(out, set->traits[i].note); fputs("</note>\n", out);
        fputs("    </trait>\n", out);
    }
    fputs("  </traits>\n", out);

    fputs("  <ageBands>\n", out);
    for (i = 0u; i < set->band_count; ++i) {
        const sl_age_band *b = &set->bands[i];
        fprintf(out, "    <ageBand lower=\"%d\" upper=\"%d\" populationMillions=\"%.0f\""
                     " mentioned=\"%s\" basicAnatomyCheckin=\"%s\" route=\"%s\">",
                b->lower, b->upper, b->population_millions,
                b->mentioned ? "true" : "false",
                b->basic_anatomy_checkin ? "true" : "false",
                sl_age_route_string(b->route));
        xml_escape(out, b->label);
        fputs("</ageBand>\n", out);
    }
    fputs("  </ageBands>\n", out);
    fputs("</intSet>\n", out);
    return 1;
}
