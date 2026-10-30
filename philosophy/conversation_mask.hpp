#ifndef SLEELA_PHILOSOPHY_CONVERSATION_MASK_HPP
#define SLEELA_PHILOSOPHY_CONVERSATION_MASK_HPP

#include "screen_map.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace sleela::philosophy {

struct ScreenMap {
    std::vector<sm_observation> observations;
    std::size_t omitted_private = 0;
    std::size_t omitted_by_domain = 0;
    std::size_t omitted_by_policy = 0;
    bool unknowns_left_unknown = true;
};

class ConversationMask {
public:
    ConversationMask() noexcept;
    virtual ~ConversationMask() = default;

    ConversationMask(const ConversationMask &) = delete;
    ConversationMask &operator=(const ConversationMask &) = delete;
    ConversationMask(ConversationMask &&) = delete;
    ConversationMask &operator=(ConversationMask &&) = delete;

    sm_status record(const sm_observation &observation) noexcept;
    sm_status record(
        const char *key, const char *unit, double value,
        std::uint64_t observed_at, sm_domain domain, sm_source source,
        double confidence, std::uint32_t relationship_group,
        bool share_with_conversation) noexcept;

    ScreenMap buildScreenMap(std::uint32_t allowed_domains = 0u) const;
    const sm_profile &profile() const noexcept;

    static sm_status reduce(
        double value, int minimum, int maximum,
        sm_rounding rounding, sm_reduction *out) noexcept;

protected:
    virtual bool permits(const sm_observation &observation) const noexcept;

private:
    sm_profile profile_;
};

class LearningConversationMask final : public ConversationMask {
protected:
    bool permits(const sm_observation &observation) const noexcept override;
};

class MotorTaskConversationMask final : public ConversationMask {
protected:
    bool permits(const sm_observation &observation) const noexcept override;
};

} // namespace sleela::philosophy

#endif
