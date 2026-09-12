#ifndef SLEELA_PCC_H
#define SLEELA_PCC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PCC_ID_BYTES 16
#define PCC_COMMITMENT_BYTES 32
#define PCC_SIGNATURE_MAX 512

typedef enum {
    PCC_CLASS_NONE = 0,
    PCC_CLASS_ORIGIN = 1,
    PCC_CLASS_COUNTRY = 2,
    PCC_CLASS_BORDER = 3,
    PCC_CLASS_REGION = 4,
    PCC_CLASS_MULTI_COUNTRY = 5,
    PCC_CLASS_FULL = 6
} pcc_registration_class;

typedef enum {
    PCC_ROLE_ORIGIN = 0,
    PCC_ROLE_BORDER,
    PCC_ROLE_TRANSIT,
    PCC_ROLE_COUNTRY,
    PCC_ROLE_REGIONAL,
    PCC_ROLE_SOCIAL_SYSTEM,
    PCC_ROLE_DESTINATION,
    PCC_ROLE_SCIENCE,
    PCC_ROLE_SCIENCE_HUB
} pcc_router_role;

typedef struct {
    uint8_t pcc_id[PCC_ID_BYTES];
    uint8_t circuit_id[PCC_ID_BYTES];
    uint64_t epoch;
    uint64_t sequence;
    pcc_registration_class registration_class;
    pcc_router_role requested_role;
    uint8_t packet_commitment[PCC_COMMITMENT_BYTES];
    uint64_t expiration;
} pcc_registration_request;

typedef struct {
    uint8_t pcc_id[PCC_ID_BYTES];
    uint8_t circuit_id[PCC_ID_BYTES];
    uint8_t router_id[PCC_ID_BYTES];
    uint64_t epoch;
    uint64_t sequence;
    pcc_router_role router_role;
    uint64_t observation_time;
    uint8_t packet_commitment[PCC_COMMITMENT_BYTES];
    uint8_t receipt_commitment[PCC_COMMITMENT_BYTES];
    uint8_t signature[PCC_SIGNATURE_MAX];
    size_t signature_len;
} pcc_receipt;

int pcc_commitment_sha256(const uint8_t *data, size_t data_len,
                          uint8_t out[PCC_COMMITMENT_BYTES]);

int pcc_request_commitment(const pcc_registration_request *request,
                           uint8_t out[PCC_COMMITMENT_BYTES]);

int pcc_receipt_signing_input(const pcc_receipt *receipt,
                              uint8_t *out, size_t out_capacity,
                              size_t *out_len);

int pcc_receipt_validate(const pcc_receipt *receipt, uint64_t now);

#ifdef __cplusplus
}
#endif

#endif
