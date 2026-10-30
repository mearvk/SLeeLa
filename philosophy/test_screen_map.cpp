#include "conversation_mask.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>

using namespace sleela::philosophy;

static sm_observation make_observation(
    const char *key, const char *unit, double value,
    std::uint64_t timestamp, sm_domain domain,
    sm_source source = SM_SOURCE_SELF_REPORT, bool share = true) {

    sm_observation observation{};
    assert(sm_observation_init(
        &observation, key, unit, value, timestamp, domain, source,
        0.8, 0u, share) == SM_STATUS_OK);
    return observation;
}

int main() {
    sm_reduction reduced{};

    assert(sm_reduce_integer(
        2.5, -10, 10, SM_ROUND_NEAREST_TIES_AWAY_FROM_ZERO,
        &reduced) == SM_STATUS_OK);
    assert(reduced.valid && reduced.value == 3 && !reduced.was_clamped);

    assert(sm_reduce_integer(
        -2.5, -10, 10, SM_ROUND_NEAREST_TIES_AWAY_FROM_ZERO,
        &reduced) == SM_STATUS_OK);
    assert(reduced.value == -3);

    assert(sm_reduce_integer(
        120.0, 0, 100, SM_ROUND_FLOOR, &reduced) == SM_STATUS_OK);
    assert(reduced.value == 100 && reduced.was_clamped);

    assert(sm_reduce_integer(
        std::numeric_limits<double>::quiet_NaN(), 0, 100,
        SM_ROUND_FLOOR, &reduced) == SM_STATUS_INVALID_NUMBER);
    assert(!reduced.valid);

    assert(sm_reduce_ratio(
        9.0, 10.0, 100.0, 0, 100,
        SM_ROUND_NEAREST_TIES_AWAY_FROM_ZERO, &reduced) == SM_STATUS_OK);
    assert(reduced.value == 90);

    assert(sm_reduce_ratio(
        1.0, 0.0, 100.0, 0, 100,
        SM_ROUND_NEAREST_TIES_AWAY_FROM_ZERO, &reduced)
        == SM_STATUS_INVALID_NUMBER);

    sm_observation before = make_observation(
        "learning_accuracy", "proportion", 0.70, 100u, SM_DOMAIN_LEARNING);
    sm_observation after = make_observation(
        "learning_accuracy", "proportion", 0.85, 200u, SM_DOMAIN_LEARNING);
    sm_observation wrong_unit = make_observation(
        "learning_accuracy", "percent", 85.0, 300u, SM_DOMAIN_LEARNING);
    double delta = 0.0;

    assert(sm_observation_delta(&before, &after, &delta) == SM_STATUS_OK);
    assert(std::fabs(delta - 0.15) < 1e-12);
    assert(sm_observation_delta(&before, &wrong_unit, &delta)
           == SM_STATUS_INCOMPATIBLE_OBSERVATIONS);

    sm_profile profile{};
    sm_profile_init(&profile);
    assert(sm_profile_add(&profile, &before) == SM_STATUS_OK);
    assert(sm_profile_add(&profile, &after) == SM_STATUS_OK);

    sm_observation private_note = make_observation(
        "private_note", "flag", 1.0, 150u, SM_DOMAIN_RELATIONSHIP,
        SM_SOURCE_SELF_REPORT, false);
    assert(sm_profile_add(&profile, &private_note) == SM_STATUS_OK);

    sm_observation animal{};
    assert(sm_observation_init(
        &animal, "mouse_assay", "unit", 0.4, 300u,
        SM_DOMAIN_HEALTH, SM_SOURCE_ANIMAL_MODEL, 0.9, 0u, true)
        == SM_STATUS_INVALID_ARGUMENT);
    assert(sm_observation_init(
        &animal, "mouse_assay", "unit", 0.4, 300u,
        SM_DOMAIN_EVIDENCE, SM_SOURCE_ANIMAL_MODEL, 0.9, 0u, true)
        == SM_STATUS_OK);
    assert(sm_profile_add(&profile, &animal) == SM_STATUS_OK);

    sm_screen_map screen{};
    assert(sm_screen_map_build(&profile, 0u, &screen) == SM_STATUS_OK);
    assert(screen.unknowns_left_unknown);
    assert(screen.omitted_private == 1u);
    assert(screen.count == 3u);

    assert(sm_screen_map_build(
        &profile, SM_DOMAIN_BIT(SM_DOMAIN_LEARNING), &screen) == SM_STATUS_OK);
    assert(screen.count == 2u);
    assert(screen.omitted_by_domain == 1u);
    assert(screen.omitted_private == 1u);

    LearningConversationMask learning_mask;
    assert(learning_mask.record(before) == SM_STATUS_OK);
    assert(learning_mask.record(after) == SM_STATUS_OK);
    assert(learning_mask.record(private_note) == SM_STATUS_OK);
    const ScreenMap derived = learning_mask.buildScreenMap();
    assert(derived.observations.size() == 2u);
    assert(derived.omitted_private == 1u);

    MotorTaskConversationMask motor_mask;
    assert(motor_mask.record(before) == SM_STATUS_OK);
    assert(motor_mask.record(private_note) == SM_STATUS_OK);
    const ScreenMap motor = motor_mask.buildScreenMap();
    assert(motor.observations.empty());
    assert(motor.omitted_private == 1u);
    assert(motor.omitted_by_policy == 1u);
    return 0;
}
