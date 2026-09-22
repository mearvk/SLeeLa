#include "../../core/sleela_design_activity.h"
#include <assert.h>
#include <math.h>
#include <string.h>
int main(void) {
    double raw[SLDA_DIMENSIONS] = {100,95,90,85,80,75};
    SLDA_Vector normalized, expected;
    SLDA_Result result = {0};
    assert(slda_normalize(raw, &normalized) == 0);
    assert(fabs(normalized.value[0] - 1.0) < 1e-12);
    assert(slda_science_profile(SLDA_SCIENCE_PHYSICS, &expected) == 0);
    assert(slda_compare(&normalized, &expected, &result) == 0);
    assert(result.mean_squared_variance >= 0.0);
    assert(isfinite(result.mean_squared_variance));
    double bad[SLDA_DIMENSIONS] = {0,0,0,0,0,NAN};
    assert(slda_normalize(bad, &normalized) != 0);
    assert(slda_parse_domain("physics", &result.domain) == 0);
    char json[256];
    assert(slda_format_json("sleela", &result, json, sizeof json) > 0);
    assert(strstr(json, ""science":"physics"") != NULL);
    return 0;
}
