#include "pcc.h"

#include <openssl/evp.h>
#include <string.h>

static void put_u64_be(uint8_t out[8], uint64_t value) {
    for (int i = 7; i >= 0; --i) {
        out[i] = (uint8_t)(value & 0xffU);
        value >>= 8;
    }
}

int pcc_commitment_sha256(const uint8_t *data, size_t data_len,
                          uint8_t out[PCC_COMMITMENT_BYTES]) {
    EVP_MD_CTX *ctx;
    unsigned int len = 0;
    static const uint8_t domain[] = "SLeeLa/PCC/commitment/v1";

    if ((!data && data_len) || !out) {
        return 0;
    }

    ctx = EVP_MD_CTX_new();
    if (!ctx) {
        return 0;
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1 ||
        EVP_DigestUpdate(ctx, domain, sizeof(domain) - 1) != 1 ||
        (data_len && EVP_DigestUpdate(ctx, data, data_len) != 1) ||
        EVP_DigestFinal_ex(ctx, out, &len) != 1 ||
        len != PCC_COMMITMENT_BYTES) {
        EVP_MD_CTX_free(ctx);
        return 0;
    }

    EVP_MD_CTX_free(ctx);
    return 1;
}

int pcc_request_commitment(const pcc_registration_request *request,
                           uint8_t out[PCC_COMMITMENT_BYTES]) {
    uint8_t canonical[16 + 16 + 8 + 8 + 1 + 1 + 32 + 8];
    size_t p = 0;

    if (!request || !out) {
        return 0;
    }

    memcpy(canonical + p, request->pcc_id, PCC_ID_BYTES);
    p += PCC_ID_BYTES;
    memcpy(canonical + p, request->circuit_id, PCC_ID_BYTES);
    p += PCC_ID_BYTES;
    put_u64_be(canonical + p, request->epoch);
    p += 8;
    put_u64_be(canonical + p, request->sequence);
    p += 8;
    canonical[p++] = (uint8_t)request->registration_class;
    canonical[p++] = (uint8_t)request->requested_role;
    memcpy(canonical + p, request->packet_commitment, PCC_COMMITMENT_BYTES);
    p += PCC_COMMITMENT_BYTES;
    put_u64_be(canonical + p, request->expiration);
    p += 8;

    return pcc_commitment_sha256(canonical, p, out);
}

int pcc_receipt_signing_input(const pcc_receipt *receipt,
                              uint8_t *out, size_t out_capacity,
                              size_t *out_len) {
    const size_t needed = 16 + 16 + 16 + 8 + 8 + 1 + 8 + 32;
    size_t p = 0;

    if (!receipt || !out || !out_len || out_capacity < needed) {
        return 0;
    }

    memcpy(out + p, receipt->pcc_id, PCC_ID_BYTES);
    p += PCC_ID_BYTES;
    memcpy(out + p, receipt->circuit_id, PCC_ID_BYTES);
    p += PCC_ID_BYTES;
    memcpy(out + p, receipt->router_id, PCC_ID_BYTES);
    p += PCC_ID_BYTES;
    put_u64_be(out + p, receipt->epoch);
    p += 8;
    put_u64_be(out + p, receipt->sequence);
    p += 8;
    out[p++] = (uint8_t)receipt->router_role;
    put_u64_be(out + p, receipt->observation_time);
    p += 8;
    memcpy(out + p, receipt->packet_commitment, PCC_COMMITMENT_BYTES);
    p += PCC_COMMITMENT_BYTES;

    *out_len = p;
    return 1;
}

int pcc_receipt_validate(const pcc_receipt *receipt, uint64_t now) {
    if (!receipt || receipt->signature_len == 0 ||
        receipt->signature_len > PCC_SIGNATURE_MAX ||
        receipt->observation_time > now + 300) {
        return 0;
    }

    return 1;
}
