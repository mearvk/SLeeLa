#include "conversation_mask.hpp"

#include <cstdint>
#include <iostream>

int main() {
    using namespace sleela::philosophy;
    constexpr std::uint64_t first_day = UINT64_C(1780000000);

    LearningConversationMask learning_view;
    MotorTaskConversationMask motor_view;

    const auto record_both = [&](const char *key, const char *unit,
                                 double value, std::uint64_t timestamp,
                                 sm_domain domain, bool share) {
        const sm_status a = learning_view.record(
            key, unit, value, timestamp, domain, SM_SOURCE_SELF_REPORT,
            0.9, 7u, share);
        const sm_status b = motor_view.record(
            key, unit, value, timestamp, domain, SM_SOURCE_SELF_REPORT,
            0.9, 7u, share);
        return a == SM_STATUS_OK && b == SM_STATUS_OK;
    };

    if (!record_both("course_completion", "proportion", 0.80, first_day,
                     SM_DOMAIN_LEARNING, true) ||
        !record_both("course_completion", "proportion", 0.92,
                     first_day + UINT64_C(7776000),
                     SM_DOMAIN_LEARNING, true) ||
        !record_both("motor_task_time", "seconds", 18.4, first_day,
                     SM_DOMAIN_MOTOR_TASK, true) ||
        !record_both("private_relationship_note", "flag", 1.0, first_day,
                     SM_DOMAIN_RELATIONSHIP, false)) {
        std::cerr << "Could not record the example observations.\n";
        return 1;
    }

    const ScreenMap learning = learning_view.buildScreenMap();
    const ScreenMap motor = motor_view.buildScreenMap();

    std::cout << "Learning view: " << learning.observations.size()
              << " visible record(s); private omitted: "
              << learning.omitted_private
              << "; other domains omitted by inheritance: "
              << learning.omitted_by_policy << '\n';

    std::cout << "Motor-task view: " << motor.observations.size()
              << " visible record(s); private omitted: "
              << motor.omitted_private
              << "; other domains omitted by inheritance: "
              << motor.omitted_by_policy << '\n';
    return 0;
}
