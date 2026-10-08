#ifndef SLEELA_AMAZON_HPP
#define SLEELA_AMAZON_HPP

#include "amazon.h"
#include <string>
#include <vector>

namespace sleela::amazon {

struct Candidate {
    std::string id;
    std::string role;
    double skillFit{0.0};
    double performance{0.0};
    double trainingNeed{0.0};
    double retentionValue{0.0};
    bool voluntaryTransition{false};
};

struct Recommendation {
    sl_amazon_action action{SL_AMAZON_RETAIN};
    double score{0.0};
    bool humanReview{true};
    bool policyOK{false};
    std::string reason;
};

class HiringManager {
public:
    Recommendation evaluate(const Candidate&) const;
};

class WorkforceManager {
public:
    Recommendation manage(const Candidate&, sl_amazon_action) const;
};

class CollegeManager {
public:
    std::vector<std::string> coursesFor(const Candidate&) const;
};

class GearManager {
public:
    bool assign(const std::string& personId, const std::string& gearId) const;
};

class FriendsManager {
public:
    bool healthyTeamConnection(const std::string& a,
                               const std::string& b) const;
};

class WorkplaceLaw {
public:
    bool review(const std::string& jurisdiction,
                sl_amazon_action action,
                bool voluntary) const;
};

class InterTrireManager {
public:
    Recommendation transition(const Candidate&, bool voluntary) const;
};

} // namespace sleela::amazon
#endif
