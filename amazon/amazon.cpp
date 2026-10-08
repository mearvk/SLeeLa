#include "amazon.hpp"

namespace sleela::amazon {

Recommendation HiringManager::evaluate(const Candidate& c) const {
    Recommendation r;
    r.action = SL_AMAZON_HIRE;
    r.score = c.skillFit * 0.55 + c.performance * 0.20 +
              c.retentionValue * 0.25;
    r.humanReview = true;
    r.policyOK = true;
    r.reason = "criteria-based hiring review; human approval required";
    return r;
}

Recommendation WorkforceManager::manage(const Candidate& c,
                                         sl_amazon_action action) const {
    Recommendation r;
    sl_amazon_profile p{c.id.c_str(), c.role.c_str(), c.skillFit,
                         c.performance, c.trainingNeed, c.retentionValue,
                         c.voluntaryTransition ? 1 : 0};
    sl_amazon_recommendation native{};
    if (sl_amazon_evaluate(&p, action, &native) == 0) {
        r.action = native.action;
        r.score = native.score;
        r.humanReview = native.requires_human_review != 0;
        r.policyOK = native.policy_ok != 0;
        r.reason = native.reason ? native.reason : "review";
    } else {
        r.reason = "invalid workforce profile";
        r.policyOK = false;
    }
    return r;
}

std::vector<std::string> CollegeManager::coursesFor(const Candidate& c) const {
    if (c.trainingNeed >= 0.75)
        return {"Safety", "Role Fundamentals", "Communication", "Workplace Law"};
    if (c.trainingNeed >= 0.40)
        return {"Role Refresh", "Communication", "Safety"};
    return {"Leadership", "Mentoring"};
}

bool GearManager::assign(const std::string& personId,
                         const std::string& gearId) const {
    return !personId.empty() && !gearId.empty();
}

bool FriendsManager::healthyTeamConnection(const std::string& a,
                                           const std::string& b) const {
    return !a.empty() && !b.empty() && a != b;
}

bool WorkplaceLaw::review(const std::string& jurisdiction,
                          sl_amazon_action action,
                          bool voluntary) const {
    return sl_amazon_policy_check(jurisdiction.c_str(), action,
                                  voluntary ? 1 : 0) != 0;
}

Recommendation InterTrireManager::transition(const Candidate& c,
                                              bool voluntary) const {
    Recommendation r;
    r.action = SL_AMAZON_RETIRE;
    r.score = c.retentionValue;
    r.humanReview = true;
    r.policyOK = voluntary;
    r.reason = "succession, redeployment, or retirement review; never an IQ test";
    return r;
}

} // namespace sleela::amazon
