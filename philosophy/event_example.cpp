#include "social_listener.hpp"
#include <cstdint>
#include <iostream>
int main(){
    using namespace sleela::philosophy;constexpr std::uint64_t now=UINT64_C(1793400000);
    EconomicSocialListener economy;MedicalSocialListener medical;sl_event e{};
    if(sl_event_init_real(&e,1,SL_EVENT_ECONOMIC_METRIC,SL_SOURCE_MEDIA_OR_PUBLICATION,
       "industrial_growth","percent",2.4,now-3600u,604800u,0,true,
       "named broadcast report","Illustrative published metric")!=SL_STATUS_OK ||
       economy.submit(e)!=SL_STATUS_OK)return 1;
    if(sl_event_init_integer(&e,2,SL_EVENT_MEDICAL_REQUEST,SL_SOURCE_SELF_REPORT,
       "consultation_request","count",1,now-120u,86400u,0,true,
       "person entered","Request for a qualified assessor's opinion")!=SL_STATUS_OK ||
       medical.submit(e)!=SL_STATUS_OK)return 1;
    if(sl_event_init_integer(&e,3,SL_EVENT_ASSESSOR_OPINION,SL_SOURCE_ASSESSOR_AUTHORED,
       "assessor_opinion_record","count",1,now,2592000u,2,true,
       "assessor supplied reference","")!=SL_STATUS_OK ||
       sl_event_set_assessor_opinion(&e,"assessor (self-attested)",
       "Illustrative placeholder: replace with assessor-authored text.",true)!=SL_STATUS_OK ||
       medical.submit(e)!=SL_STATUS_OK)return 1;
    const auto ev=economy.listen(now);const auto mv=medical.listen(now);
    std::cout<<"Economic events: "<<ev.size()<<'\n'<<"Medical events: "<<mv.size()<<'\n';
    for(const auto &v:mv){
        std::cout<<sl_event_type_string(v.event.type)<<" | "
                 <<sl_source_kind_string(v.event.source)<<" | relevance "
                 <<v.relevance<<" | first-person "<<(v.first_person_report?"yes":"no")
                 <<" | revisit "<<(v.is_revisit?"yes":"no")
                 <<" | corroboration inferred "<<(v.corroboration_inferred?"yes":"no")<<'\n';
        if(v.event.type==SL_EVENT_ASSESSOR_OPINION)
            std::cout<<v.event.assessor_role<<": "<<v.event.note<<'\n';
    }
    return 0;
}
