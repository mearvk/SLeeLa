// SleelaTerminal(TM) shell -- L1 core model implementation.
// Original SLeeLa work (see NOTICE).

#include "core.hpp"

#include <cctype>
#include <cstdlib>
#include <unistd.h>   // environ

extern char** environ;

namespace sleela::sh {

long Value::asInt() const noexcept {
    // Parse an optional sign and leading digits; ignore trailing junk.
    std::size_t i = 0;
    const std::size_t n = text.size();
    while (i < n && std::isspace(static_cast<unsigned char>(text[i]))) ++i;
    long sign = 1;
    if (i < n && (text[i] == '+' || text[i] == '-')) {
        if (text[i] == '-') sign = -1;
        ++i;
    }
    long v = 0;
    bool any = false;
    while (i < n && std::isdigit(static_cast<unsigned char>(text[i]))) {
        v = v * 10 + (text[i] - '0');
        ++i;
        any = true;
    }
    return any ? sign * v : 0;
}

Value Value::fromInt(long v) {
    return Value(std::to_string(v));
}

Environment::Environment() {
    // Seed from the process environment so the shell starts with the caller's
    // variables (PATH etc.), each marked exported.
    for (char** e = ::environ; e && *e; ++e) {
        std::string entry(*e);
        const auto eq = entry.find('=');
        if (eq == std::string::npos) continue;
        const std::string name = entry.substr(0, eq);
        const std::string value = entry.substr(eq + 1);
        vars_[name] = Var{value, true};
    }
}

std::string Environment::get(const std::string& name) const {
    const auto it = vars_.find(name);
    return it == vars_.end() ? std::string() : it->second.value;
}

bool Environment::has(const std::string& name) const {
    return vars_.find(name) != vars_.end();
}

void Environment::set(const std::string& name, const std::string& value) {
    auto& v = vars_[name];
    v.value = value;
    // keep existing 'exported' flag if the var already existed
}

void Environment::unset(const std::string& name) {
    vars_.erase(name);
}

void Environment::exportVar(const std::string& name) {
    vars_[name].exported = true;
}

bool Environment::isExported(const std::string& name) const {
    const auto it = vars_.find(name);
    return it != vars_.end() && it->second.exported;
}

std::vector<std::string> Environment::exportedEnviron() const {
    std::vector<std::string> out;
    out.reserve(vars_.size());
    for (const auto& [name, var] : vars_) {
        if (var.exported) out.push_back(name + "=" + var.value);
    }
    return out;
}

void Environment::defineFunction(const std::string& name, std::shared_ptr<Node> body) {
    functions_[name] = std::move(body);
}

std::shared_ptr<Node> Environment::lookupFunction(const std::string& name) const {
    const auto it = functions_.find(name);
    return it == functions_.end() ? nullptr : it->second;
}

bool Environment::hasFunction(const std::string& name) const {
    return functions_.find(name) != functions_.end();
}

void Environment::setPositionals(std::vector<std::string> args) {
    positionals_ = std::move(args);
}

std::string Environment::getPositional(std::size_t n) const {
    // 1-based: $1 is positionals_[0].
    if (n == 0 || n > positionals_.size()) return std::string();
    return positionals_[n - 1];
}

int Environment::addJob(long pid, const std::string& command) {
    Job j;
    j.id = next_job_id_++;
    j.pid = pid;
    j.command = command;
    j.running = true;
    jobs_.push_back(j);
    return j.id;
}

Environment::Job* Environment::findJob(int id) noexcept {
    for (auto& j : jobs_) if (j.id == id) return &j;
    return nullptr;
}

void Environment::removeJob(int id) noexcept {
    for (auto it = jobs_.begin(); it != jobs_.end(); ++it) {
        if (it->id == id) { jobs_.erase(it); return; }
    }
}

} // namespace sleela::sh
