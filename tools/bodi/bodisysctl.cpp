#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "gardulus_ii.h"

class BodiSysCtl {
public:
    explicit BodiSysCtl(int intervalMs) : intervalMs_(intervalMs) {}
    int run(int iterations, const std::string& logPath) {
        if (!logPath.empty()) log_ = std::fopen(logPath.c_str(), "a");
        int rc = gardulus_run(intervalMs_, iterations, &BodiSysCtl::onSample, this);
        if (log_) std::fclose(log_);
        return rc;
    }
private:
    static int onSample(const gardulus_sample* sample, void* context) {
        BodiSysCtl* self = static_cast<BodiSysCtl*>(context);
        char json[1024];
        int n = gardulus_emit_json(sample, json, sizeof(json));
        if (n < 0) return 1;
        if (self->log_) std::fwrite(json, 1, static_cast<size_t>(n), self->log_);
        else std::fwrite(json, 1, static_cast<size_t>(n), stdout);
        return 0;
    }
    int intervalMs_;
    FILE* log_ = nullptr;
};

int main(int argc, char** argv) {
    int interval = argc > 1 ? std::atoi(argv[1]) : 1000;
    int iterations = argc > 2 ? std::atoi(argv[2]) : 0;
    std::string log = argc > 3 ? argv[3] : std::string();
    if (interval < 1) return 2;
    BodiSysCtl ctl(interval);
    return ctl.run(iterations, log);
}
