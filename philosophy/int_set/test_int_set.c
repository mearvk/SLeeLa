#include "int_set.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_schema(void) {
    const sl_int_set *s = sl_int_set_default();
    size_t i;
    assert(s);
    /* All 16 authored traits present, in order. */
    assert(s->trait_count == 16u);
    assert(strcmp(s->traits[0].name, "Start") == 0);
    assert(strcmp(s->traits[15].name, "Scent of IQ") == 0);
    /* Every trait's index sits inside the declared ordinal range. */
    for (i = 0u; i < s->trait_count; ++i) {
        assert(s->traits[i].int_index >= s->range.low);
        assert(s->traits[i].int_index <= s->range.high);
    }
    /* The INT range is explicitly NOT psychometric. */
    assert(!s->range.psychometric);
    /* Thresholds as specified. */
    assert(s->not_mentioned_below == 16);
    assert(s->anatomy_checkin_above == 85);
    assert(s->start_age >= 16); /* starts at a natural, mentioned age */
}

static void test_age_routing(void) {
    const sl_int_set *s = sl_int_set_default();
    /* Younger than 16 => not mentioned. */
    assert(sl_int_set_route_for_age(s, 10, false) == SL_ROUTE_NOT_MENTIONED);
    assert(sl_int_set_route_for_age(s, 15, false) == SL_ROUTE_NOT_MENTIONED);
    /* 16 and up, up to 85 => ordinary. */
    assert(sl_int_set_route_for_age(s, 16, false) == SL_ROUTE_NONE);
    assert(sl_int_set_route_for_age(s, 85, false) == SL_ROUTE_NONE);
    /* Older than 85 => routed out by Fellows or Worship. */
    assert(sl_int_set_route_for_age(s, 86, false) == SL_ROUTE_FELLOWS);
    assert(sl_int_set_route_for_age(s, 99, true) == SL_ROUTE_WORSHIP);
}

static void test_age_bands_real_figures(void) {
    const sl_int_set *s = sl_int_set_default();
    double sum = 0.0;
    size_t i;
    /* The populated bands carry the real UN DYB 2024 world figures. */
    for (i = 0u; i < s->band_count; ++i) sum += s->bands[i].population_millions;
    /* 2017 + 5312 + 0 + 833 = 8162 (world total, millions). */
    assert(sum == 8162.0);
    /* 0-14 band is below 16 => not mentioned. */
    assert(!s->bands[0].mentioned);
    /* 85+ band => basic-anatomy check-in and Fellows routing. */
    assert(s->bands[s->band_count - 1u].basic_anatomy_checkin);
    assert(s->bands[s->band_count - 1u].route == SL_ROUTE_FELLOWS);
}

/* Minimal well-formedness checks on the emitted text. */
static char buf[16384];
static void capture(int (*fn)(const sl_int_set *, FILE *)) {
    const sl_int_set *s = sl_int_set_default();
    FILE *f = tmpfile();
    long n;
    assert(f);
    assert(fn(s, f) == 1);
    fflush(f);
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    assert(n > 0 && (size_t)n < sizeof(buf));
    fseek(f, 0, SEEK_SET);
    assert(fread(buf, 1u, (size_t)n, f) == (size_t)n);
    buf[n] = '\0';
    fclose(f);
}

static void test_json_output(void) {
    capture(sl_int_set_write_json);
    assert(buf[0] == '{');
    assert(strstr(buf, "\"Scent of IQ\""));
    assert(strstr(buf, "\"psychometric\": false"));
    assert(strstr(buf, "\"notMentionedBelow\": 16"));
    assert(strstr(buf, "\"basicAnatomyCheckinAbove\": 85"));
    assert(strstr(buf, "Fellows") && strstr(buf, "Worship"));
    assert(strstr(buf, "Winning Runs for Home Team"));
}

static void test_xml_output(void) {
    capture(sl_int_set_write_xml);
    assert(strstr(buf, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
    assert(strstr(buf, "<intSet"));
    assert(strstr(buf, "</intSet>"));
    assert(strstr(buf, "psychometric=\"false\""));
    assert(strstr(buf, "<handler>Fellows</handler>"));
    assert(strstr(buf, "<handler>Worship</handler>"));
    /* "A-ness" has no special chars; ampersand escaping still must hold. */
    assert(strstr(buf, "A-ness"));
}

int main(void) {
    printf("test_schema...\n");                 test_schema();
    printf("test_age_routing...\n");             test_age_routing();
    printf("test_age_bands_real_figures...\n");  test_age_bands_real_figures();
    printf("test_json_output...\n");             test_json_output();
    printf("test_xml_output...\n");              test_xml_output();
    printf("all int_set tests passed\n");
    return 0;
}
