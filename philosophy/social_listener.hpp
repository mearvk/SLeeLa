#ifndef SLEELA_PHILOSOPHY_SOCIAL_LISTENER_HPP
#define SLEELA_PHILOSOPHY_SOCIAL_LISTENER_HPP
#include "event_tree.h"
#include <cstddef>
#include <cstdint>
#include <vector>
namespace sleela::philosophy {
struct EventView {
    sl_event event{};
    double relevance=0.0;
    bool first_person_report=false;
    bool is_revisit=false;
    bool evidence_aware_mode=true;
    bool corroboration_inferred=false;
};
class SocialListener {
public:
    explicit SocialListener(sl_listener_mode mode=SL_LISTENER_EVIDENCE_AWARE);
    virtual ~SocialListener();
    SocialListener(const SocialListener &)=delete;
    SocialListener &operator=(const SocialListener &)=delete;
    SocialListener(SocialListener &&) noexcept;
    SocialListener &operator=(SocialListener &&) noexcept;
    sl_status submit(const sl_event &) noexcept;
    sl_status endEvent(std::uint64_t id,std::uint64_t ended_at) noexcept;
    sl_status prune(std::uint64_t now,double floor,std::size_t *removed=nullptr) noexcept;
    std::vector<EventView> listen(std::uint64_t now,double floor=0.0) const;
    sl_tree_stats stats() const noexcept;
    sl_listener_mode mode() const noexcept;
protected:
    virtual bool accepts(const sl_event &) const noexcept;
private:
    struct Collection;
    static bool visitEvent(const sl_event *,double,void *);
    sl_event_tree *tree_;
    sl_listener_mode mode_;
};
class EconomicSocialListener final : public SocialListener {
public: using SocialListener::SocialListener;
protected: bool accepts(const sl_event &) const noexcept override;
};
class MedicalSocialListener final : public SocialListener {
public: using SocialListener::SocialListener;
protected: bool accepts(const sl_event &) const noexcept override;
};
}
#endif
