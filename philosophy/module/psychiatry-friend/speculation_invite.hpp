#ifndef SLEELA_PHILOSOPHY_SPECULATION_INVITE_HPP
#define SLEELA_PHILOSOPHY_SPECULATION_INVITE_HPP
/*
 * C++17 wrapper over the C speculation-invite API. Inheritable: override
 * accept() to apply an application policy on which speculations are kept
 * (for example, require a stated scope, or only keep limitation/safety notes).
 * The base policy keeps any speculation the C core already validated
 * (self-attested, with a falsifiability note, about the software).
 *
 * See speculation_invite.h for the boundaries: no diagnosis, no treatment
 * advice, no credential verification, no network I/O.
 */
#include "speculation_invite.h"
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace sleela::philosophy {

struct Speculation {
    std::uint64_t id = 0u;
    std::string author;
    std::string role;             /* self-reported */
    std::string scope;
    std::string text;
    std::string falsifiability;
    sl_spec_aspect aspect = SL_ASPECT_LIMITATION;
    sl_spec_confidence confidence = SL_CONF_TENTATIVE;
};

class SpeculationInvite {
public:
    explicit SpeculationInvite(std::size_t capacity = SL_SPEC_DEFAULT_CAPACITY);
    virtual ~SpeculationInvite();
    SpeculationInvite(const SpeculationInvite &) = delete;
    SpeculationInvite &operator=(const SpeculationInvite &) = delete;
    SpeculationInvite(SpeculationInvite &&) noexcept;
    SpeculationInvite &operator=(SpeculationInvite &&) noexcept;

    static std::string invitation();
    static std::string disclaimer();

    /* Submit a professional's speculation. Returns the C status. A submission
     * that the C core validates but accept() rejects returns SL_SPEC_OK but is
     * not stored; check count() to confirm storage, or use submitChecked(). */
    sl_spec_status submit(const Speculation &s);
    /* Convenience: returns true only if stored (validated AND accepted). */
    bool submitChecked(const Speculation &s);

    std::size_t count() const noexcept;
    std::vector<Speculation> all() const;

    /* Careful scientific report to a stream. */
    void writeReport(std::ostream &os) const;

protected:
    /* Policy hook. Base: accept everything the C core validated. */
    virtual bool accept(const Speculation &s) const noexcept;

private:
    sl_spec_registry *reg_;
};

/* A policy that only keeps speculations about LIMITATION or SAFETY -- the
 * reach the software should NOT have -- which is often the most useful adult
 * psychiatry commentary to collect carefully. */
class BoundaryFocusedInvite final : public SpeculationInvite {
public:
    using SpeculationInvite::SpeculationInvite;
protected:
    bool accept(const Speculation &s) const noexcept override;
};

} // namespace sleela::philosophy
#endif
