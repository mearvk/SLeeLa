#include "conversation_mask.hpp"

namespace sleela::philosophy {

ConversationMask::ConversationMask() noexcept {
    sm_profile_init(&profile_);
}

sm_status ConversationMask::record(const sm_observation &observation) noexcept {
    return sm_profile_add(&profile_, &observation);
}

sm_status ConversationMask::record(
    const char *key, const char *unit, double value,
    std::uint64_t observed_at, sm_domain domain, sm_source source,
    double confidence, std::uint32_t relationship_group,
    bool share_with_conversation) noexcept {

    sm_observation observation{};
    const sm_status init_status = sm_observation_init(
        &observation, key, unit, value, observed_at, domain, source,
        confidence, relationship_group, share_with_conversation);
    if (init_status != SM_STATUS_OK) {
        return init_status;
    }
    return record(observation);
}

ScreenMap ConversationMask::buildScreenMap(std::uint32_t allowed_domains) const {
    sm_screen_map filtered{};
    ScreenMap result;

    const sm_status status =
        sm_screen_map_build(&profile_, allowed_domains, &filtered);
    if (status != SM_STATUS_OK) {
        return result;
    }

    result.omitted_private = filtered.omitted_private;
    result.omitted_by_domain = filtered.omitted_by_domain;
    result.unknowns_left_unknown = filtered.unknowns_left_unknown;
    result.observations.reserve(filtered.count);

    for (std::size_t index = 0; index < filtered.count; ++index) {
        const sm_observation &observation = filtered.observations[index];
        if (permits(observation)) {
            result.observations.push_back(observation);
        } else {
            ++result.omitted_by_policy;
        }
    }
    return result;
}

const sm_profile &ConversationMask::profile() const noexcept {
    return profile_;
}

sm_status ConversationMask::reduce(
    double value, int minimum, int maximum,
    sm_rounding rounding, sm_reduction *out) noexcept {
    return sm_reduce_integer(value, minimum, maximum, rounding, out);
}

bool ConversationMask::permits(const sm_observation &) const noexcept {
    return true;
}

bool LearningConversationMask::permits(
    const sm_observation &observation) const noexcept {
    return observation.domain == SM_DOMAIN_LEARNING;
}

bool MotorTaskConversationMask::permits(
    const sm_observation &observation) const noexcept {
    return observation.domain == SM_DOMAIN_MOTOR_TASK;
}

} // namespace sleela::philosophy
