#include "cryptographic_capsule_set.h"

#include <string.h>

int http3_capsule_set_add(http3_capsule_set_t *set,
                          const http3_jurisdiction_recipient_t *recipient)
{
    size_t i;

    if (set == NULL || recipient == NULL || recipient->jurisdiction_id[0] == '\0') {
        return -1;
    }
    if (set->recipient_count >= HTTP3_CAPSULE_MAX_JURISDICTIONS) {
        return -1;
    }

    for (i = 0U; i < set->recipient_count; ++i) {
        if (strcmp(set->recipients[i].jurisdiction_id,
                   recipient->jurisdiction_id) == 0) {
            return -1;
        }
    }

    set->recipients[set->recipient_count++] = *recipient;
    return 0;
}

int http3_capsule_set_has(const http3_capsule_set_t *set,
                          const char *jurisdiction_id)
{
    size_t index;
    return http3_capsule_set_authorized_index(set, jurisdiction_id, &index);
}

int http3_capsule_set_authorized_index(const http3_capsule_set_t *set,
                                       const char *jurisdiction_id,
                                       size_t *index)
{
    size_t i;

    if (set == NULL || jurisdiction_id == NULL || index == NULL) {
        return -1;
    }

    for (i = 0U; i < set->recipient_count; ++i) {
        if (strcmp(set->recipients[i].jurisdiction_id, jurisdiction_id) == 0) {
            *index = i;
            return 0;
        }
    }
    return -1;
}
