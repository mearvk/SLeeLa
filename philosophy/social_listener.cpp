#include "social_listener.hpp"
#include <utility>
namespace sleela::philosophy {
struct SocialListener::Collection { const SocialListener *owner; std::vector<EventView> *items; };
SocialListener::SocialListener(sl_listener_mode mode):tree_(sl_event_tree_create()),mode_(mode){
    if(mode_!=SL_LISTENER_NEUTRAL && mode_!=SL_LISTENER_EVIDENCE_AWARE)mode_=SL_LISTENER_EVIDENCE_AWARE;
}
SocialListener::~SocialListener(){sl_event_tree_destroy(tree_);}
SocialListener::SocialListener(SocialListener &&o) noexcept:tree_(o.tree_),mode_(o.mode_){o.tree_=nullptr;}
SocialListener &SocialListener::operator=(SocialListener &&o) noexcept{
    if(this!=&o){sl_event_tree_destroy(tree_);tree_=o.tree_;mode_=o.mode_;o.tree_=nullptr;}return *this;
}
sl_status SocialListener::submit(const sl_event &e) noexcept{return tree_?sl_event_tree_insert(tree_,&e):SL_STATUS_OUT_OF_MEMORY;}
sl_status SocialListener::endEvent(std::uint64_t id,std::uint64_t t) noexcept{return tree_?sl_event_tree_end(tree_,id,t):SL_STATUS_OUT_OF_MEMORY;}
sl_status SocialListener::prune(std::uint64_t now,double floor,std::size_t *removed) noexcept{
    std::size_t n=0u;if(!tree_)return SL_STATUS_OUT_OF_MEMORY;
    sl_status s=sl_event_tree_prune(tree_,now,floor,&n);if(removed)*removed=n;return s;
}
bool SocialListener::visitEvent(const sl_event *e,double weight,void *ctx){
    auto *c=static_cast<Collection *>(ctx);
    if(!e || !c || !c->owner || !c->items)return false;
    if(!e->consent_to_share || !c->owner->accepts(*e))return true;
    EventView v;v.event=*e;v.relevance=weight;
    v.first_person_report=e->source==SL_SOURCE_SELF_REPORT;
    v.is_revisit=e->related_event_id!=0u;
    v.evidence_aware_mode=c->owner->mode_==SL_LISTENER_EVIDENCE_AWARE;
    v.corroboration_inferred=false;c->items->push_back(v);return true;
}
std::vector<EventView> SocialListener::listen(std::uint64_t now,double floor) const{
    std::vector<EventView> out;if(!tree_)return out;Collection c{this,&out};
    (void)sl_event_tree_visit(tree_,now,floor,&SocialListener::visitEvent,&c);return out;
}
sl_tree_stats SocialListener::stats() const noexcept{return sl_event_tree_get_stats(tree_);}
sl_listener_mode SocialListener::mode() const noexcept{return mode_;}
bool SocialListener::accepts(const sl_event &) const noexcept{return true;}
bool EconomicSocialListener::accepts(const sl_event &e) const noexcept{return e.type==SL_EVENT_ECONOMIC_METRIC;}
bool MedicalSocialListener::accepts(const sl_event &e) const noexcept{
    return e.type==SL_EVENT_HEALTH_CONTEXT || e.type==SL_EVENT_MEDICAL_REQUEST ||
           e.type==SL_EVENT_ASSESSOR_OPINION;
}
}
