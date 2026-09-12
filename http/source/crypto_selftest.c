#include "http3_crypto.h"

#include <stdio.h>
#include <string.h>

static int expect_equal(const unsigned char *a, const unsigned char *b, size_t n)
{
    return memcmp(a, b, n) == 0 ? 0 : -1;
}

int main(void)
{
    unsigned char alice_private[32], alice_public[32];
    unsigned char bob_private[32], bob_public[32];
    unsigned char alice_shared[32], bob_shared[32];
    unsigned char key[32], nonce[12], tag[16];
    unsigned char ciphertext[64], plaintext[64];
    const unsigned char message[] = "SLeeLa HTTP 3.0 crypto self-test";
    const unsigned char info[] = "self-test";
    http3_cic_metadata_t metadata;
    http3_cic_capsule_t capsule;
    size_t plaintext_len = 0U;
    http3_route_plan_t old_plan;
    http3_route_plan_t new_plan;
    http3_route_session_t session;

    memset(&metadata, 0, sizeof(metadata));
    memset(&capsule, 0, sizeof(capsule));
    memset(&old_plan, 0, sizeof(old_plan));
    memset(&new_plan, 0, sizeof(new_plan));
    memset(&session, 0, sizeof(session));

    if (http3_openssl_x25519_generate(alice_private, alice_public) != 0 ||
        http3_openssl_x25519_generate(bob_private, bob_public) != 0 ||
        http3_openssl_x25519_derive(alice_private, bob_public, alice_shared) != 0 ||
        http3_openssl_x25519_derive(bob_private, alice_public, bob_shared) != 0 ||
        expect_equal(alice_shared, bob_shared, sizeof(alice_shared)) != 0) {
        return 1;
    }

    if (http3_openssl_hkdf_sha256(alice_shared, sizeof(alice_shared),
                                  NULL, 0U, info, sizeof(info) - 1U, key) != 0 ||
        http3_openssl_random(nonce, sizeof(nonce)) != 0 ||
        http3_openssl_aes256gcm_encrypt(key, nonce, NULL, 0U,
                                        message, sizeof(message), ciphertext, tag) != 0 ||
        http3_openssl_aes256gcm_decrypt(key, nonce, NULL, 0U,
                                        ciphertext, sizeof(message), tag, plaintext) != 0 ||
        expect_equal(message, plaintext, sizeof(message)) != 0) {
        return 2;
    }

    if (http3_cic_create_id(metadata.capsule_id) != 0) {
        return 3;
    }
    strcpy(metadata.jurisdiction_id, "US");
    strcpy(metadata.emergency_endpoint_reference, "registry://us/emergency");
    strcpy(metadata.sender_reference, "sender-reference");
    metadata.expiration = 1U;

    if (http3_cic_encrypt(&metadata, bob_public, message, sizeof(message), &capsule) != 0 ||
        http3_cic_decrypt(&capsule, bob_private, plaintext, sizeof(plaintext),
                          &plaintext_len) != 0 ||
        plaintext_len != sizeof(message) ||
        expect_equal(message, plaintext, sizeof(message)) != 0) {
        http3_cic_free_capsule(&capsule);
        return 4;
    }
    http3_cic_free_capsule(&capsule);

    old_plan.route_id = 100U;
    old_plan.generation = 1U;
    old_plan.geodata_epoch = 1U;
    old_plan.hop_count = 2U;
    old_plan.hops[0].node_id = 1U;
    old_plan.hops[0].geodata.country_id = 840U;
    old_plan.hops[1].node_id = 2U;
    old_plan.hops[1].geodata.country_id = 840U;

    new_plan = old_plan;
    new_plan.generation = 2U;
    new_plan.hops[1].node_id = 3U;
    new_plan.hops[1].geodata.network_id = 7U;

    if (http3_route_session_establish(&session, &old_plan, bob_public, 1U) != 0 ||
        http3_route_session_rekey(&session, &old_plan, &new_plan,
                                  bob_public, 2U) != 0 ||
        session.route_generation != 2U) {
        http3_route_session_clear(&session);
        return 5;
    }
    http3_route_session_clear(&session);

    puts("HTTP 3.0 crypto self-test: PASS");
    return 0;
}
