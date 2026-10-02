#include "sleela_decompiler.h"
#include <string>
#include <utility>
#include <vector>

namespace sleela::decompiler {

struct Evidence {
    std::string source;
    std::string value;
    int weight;
    bool conflict;
};

struct Module {
    std::string language;
    std::string version;
    std::string format;
    std::string os;
    bool trusted;
};

class Pipeline {
public:
    explicit Pipeline(const sleela_decompiler_request &request) : request_(request) {}
    bool validate() const { return sleela_decompiler_validate_request(&request_) != 0; }
    bool preserveFractional(sleela_decompiler_report &report,
                            std::size_t valid, std::size_t total) const {
        return sleela_decompiler_preserve_fractional(
            &report, valid, total, request_.fractional_policy) != 0;
    }
    void addModule(Module module) { modules_.push_back(std::move(module)); }
    std::size_t moduleCount() const { return modules_.size(); }
private:
    sleela_decompiler_request request_;
    std::vector<Module> modules_;
};

} // namespace sleela::decompiler
