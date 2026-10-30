#include "speculation_invite.hpp"
#include <cstdio>
#include <cstdlib>
#include <vector>

namespace sleela::philosophy {

SpeculationInvite::SpeculationInvite(std::size_t capacity)
    : reg_(sl_spec_registry_create(capacity)) {}

SpeculationInvite::~SpeculationInvite() { if (reg_) sl_spec_registry_destroy(reg_); }

SpeculationInvite::SpeculationInvite(SpeculationInvite &&o) noexcept : reg_(o.reg_) {
    o.reg_ = nullptr;
}
SpeculationInvite &SpeculationInvite::operator=(SpeculationInvite &&o) noexcept {
    if (this != &o) {
        if (reg_) sl_spec_registry_destroy(reg_);
        reg_ = o.reg_;
        o.reg_ = nullptr;
    }
    return *this;
}

std::string SpeculationInvite::invitation() { return sl_spec_invitation_text(); }
std::string SpeculationInvite::disclaimer() { return sl_spec_disclaimer_text(); }

bool SpeculationInvite::accept(const Speculation &) const noexcept { return true; }

sl_spec_status SpeculationInvite::submit(const Speculation &s) {
    if (!reg_) return SL_SPEC_INVALID_ARGUMENT;
    sl_speculation raw;
    sl_spec_status st = sl_speculation_init(
        &raw, s.id, s.aspect, s.confidence,
        s.author.c_str(), s.role.empty() ? nullptr : s.role.c_str(),
        s.scope.empty() ? nullptr : s.scope.c_str(),
        s.text.c_str(), s.falsifiability.c_str(), /*self_attested=*/true);
    if (st != SL_SPEC_OK) return st;
    if (!accept(s)) return SL_SPEC_OK; /* validated but policy-filtered; not stored */
    return sl_spec_registry_submit(reg_, &raw);
}

bool SpeculationInvite::submitChecked(const Speculation &s) {
    std::size_t before = count();
    sl_spec_status st = submit(s);
    return st == SL_SPEC_OK && count() == before + 1u;
}

std::size_t SpeculationInvite::count() const noexcept {
    return sl_spec_registry_count(reg_);
}

std::vector<Speculation> SpeculationInvite::all() const {
    std::vector<Speculation> out;
    std::size_t n = count();
    out.reserve(n);
    for (std::size_t i = 0u; i < n; ++i) {
        const sl_speculation *r = sl_spec_registry_at(reg_, i);
        if (!r) continue;
        Speculation s;
        s.id = r->id;
        s.author = r->author;
        s.role = r->role;
        s.scope = r->scope;
        s.text = r->text;
        s.falsifiability = r->falsifiability;
        s.aspect = r->aspect;
        s.confidence = r->confidence;
        out.push_back(std::move(s));
    }
    return out;
}

void SpeculationInvite::writeReport(std::ostream &os) const {
    /* Reuse the C renderer via a temporary memory stream for consistency. */
    char *buf = nullptr;
    std::size_t size = 0u;
    FILE *mem =
#if defined(_WIN32)
        std::tmpfile();
#else
        open_memstream(&buf, &size);
#endif
    if (!mem) { os << invitation() << "\n\n(report unavailable)\n"; return; }
    sl_spec_registry_write_report(reg_, mem);
#if defined(_WIN32)
    std::fflush(mem);
    std::fseek(mem, 0, SEEK_END);
    long n = std::ftell(mem);
    std::fseek(mem, 0, SEEK_SET);
    std::vector<char> tmp((n > 0) ? static_cast<std::size_t>(n) : 0u);
    if (!tmp.empty()) {
        std::size_t rd = std::fread(tmp.data(), 1u, tmp.size(), mem);
        os.write(tmp.data(), static_cast<std::streamsize>(rd));
    }
    std::fclose(mem);
#else
    std::fflush(mem);
    std::fclose(mem);
    if (buf) { os.write(buf, static_cast<std::streamsize>(size)); free(buf); }
#endif
}

bool BoundaryFocusedInvite::accept(const Speculation &s) const noexcept {
    return s.aspect == SL_ASPECT_LIMITATION || s.aspect == SL_ASPECT_SAFETY;
}

} // namespace sleela::philosophy
