#ifndef SLEELA_ICC_HPP
#define SLEELA_ICC_HPP

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Project model only; not an official ICC interface or authorization. */
bool sleela_icc_us_emblem_use_allowed(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace sleela::institutions {

class ICC {
public:
    /* The ICC does not receive authority to use or reproduce U.S. government emblems. */
    static bool us_emblem_use_allowed() noexcept;
};

} // namespace sleela::institutions
#endif

#endif /* SLEELA_ICC_HPP */
