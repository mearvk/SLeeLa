#include "social_listener.hpp"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>
struct VisitLog{std::vector<std::uint64_t> ids;};
static bool collect(const sl_event *e,double,void *p){static_cast<VisitLog *>(p)->ids.push_back(e->id);return true;}
static sl_event event(std::uint64_t id,std::uint64_t at,std::uint64_t half,
    sl_event_type type=SL_EVENT_SOCIAL,sl_source_kind source=SL_SOURCE_DIRECT_OBSERVATION){
    sl_event e{};assert(sl_event_init_integer(&e,id,type,source,"metric","unit",(int64_t)id,
        at,half,0,true,"test-source","")==SL_STATUS_OK);return e;
}
int main(){
    const std::uint64_t base=1000000u;sl_event_tree *t=sl_event_tree_create();assert(t);
    for(std::uint64_t i=512u;i>0u;--i){sl_event e=event(i,base,1000u);assert(sl_event_tree_insert(t,&e)==SL_STATUS_OK);}
    sl_tree_stats s=sl_event_tree_get_stats(t);assert(s.count==512u && s.height<=20u);
    sl_event dup=event(5u,base,1000u);assert(sl_event_tree_insert(t,&dup)==SL_STATUS_DUPLICATE_ID);
    sl_event got{};assert(sl_event_tree_find(t,20u,&got)==SL_STATUS_OK && got.id==20u);
    sl_event decay=event(900u,base,100u);assert(std::fabs(sl_event_relevance(&decay,base+100u)-0.5)<1e-12);
    assert(sl_event_tree_insert(t,&decay)==SL_STATUS_OK);
    assert(sl_event_tree_end(t,900u,base+100u)==SL_STATUS_OK);
    assert(sl_event_tree_find(t,900u,&got)==SL_STATUS_OK);
    assert(std::fabs(sl_event_relevance(&got,base+200u)-0.5)<1e-12);
    sl_event revisit{};assert(sl_event_init_integer(&revisit,901u,SL_EVENT_REFLECTION,
        SL_SOURCE_SELF_REPORT,"revisit","count",1,base+200u,1000u,20u,true,
        "first-person","Remembering event 20")==SL_STATUS_OK);
    assert(sl_event_tree_insert(t,&revisit)==SL_STATUS_OK);
    VisitLog log;assert(sl_event_tree_visit(t,base+200u,0.5,collect,&log)==SL_STATUS_OK);
    for(std::size_t i=1u;i<log.ids.size();++i)assert(log.ids[i-1u]<log.ids[i]);
    std::size_t removed=0u;assert(sl_event_tree_prune(t,base+1000000u,0.1,&removed)==SL_STATUS_OK);
    assert(removed>0u && sl_event_tree_get_stats(t).count==0u);
    sl_event opinion{};assert(sl_event_init_integer(&opinion,1001u,SL_EVENT_ASSESSOR_OPINION,
        SL_SOURCE_ASSESSOR_AUTHORED,"opinion_record","record",1,base,100u,0,true,
        "assessor reference","")==SL_STATUS_OK);
    assert(sl_event_tree_insert(t,&opinion)==SL_STATUS_INVALID_EVENT);
    assert(sl_event_set_assessor_opinion(&opinion,"assessor (self-attested)",
        "Assessor-authored statement supplied by the assessor.",true)==SL_STATUS_OK);
    assert(sl_event_tree_insert(t,&opinion)==SL_STATUS_OK);
    sl_event priv=event(1002u,base,100u,SL_EVENT_MEDICAL_REQUEST,SL_SOURCE_SELF_REPORT);
    priv.consent_to_share=false;assert(sl_event_tree_insert(t,&priv)==SL_STATUS_OK);
    sl_event_tree_destroy(t);

    /* C++ listeners retain provenance, filter by event type, and honor consent. */
    EconomicSocialListener economy(SL_LISTENER_EVIDENCE_AWARE);
    MedicalSocialListener medicine(SL_LISTENER_NEUTRAL);
    sl_event economic = event(2000u, base, 1000u, SL_EVENT_ECONOMIC_METRIC,
                              SL_SOURCE_MEDIA_OR_PUBLICATION);
    sl_event request = event(2001u, base, 1000u, SL_EVENT_MEDICAL_REQUEST,
                             SL_SOURCE_SELF_REPORT);
    sl_event private_health = event(2002u, base, 1000u, SL_EVENT_HEALTH_CONTEXT,
                                    SL_SOURCE_SELF_REPORT);
    private_health.consent_to_share = false;
    assert(economy.submit(economic) == SL_STATUS_OK);
    assert(economy.submit(request) == SL_STATUS_OK);
    assert(medicine.submit(request) == SL_STATUS_OK);
    assert(medicine.submit(private_health) == SL_STATUS_OK);
    const auto economy_view = economy.listen(base);
    const auto medicine_view = medicine.listen(base);
    assert(economy_view.size() == 1u);
    assert(economy_view[0].event.type == SL_EVENT_ECONOMIC_METRIC);
    assert(economy_view[0].evidence_aware_mode);
    assert(!economy_view[0].first_person_report);
    assert(medicine_view.size() == 1u);
    assert(medicine_view[0].event.type == SL_EVENT_MEDICAL_REQUEST);
    assert(!medicine_view[0].evidence_aware_mode);
    assert(medicine_view[0].first_person_report);
    assert(!medicine_view[0].corroboration_inferred);
    return 0;
}
