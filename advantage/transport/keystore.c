/*
 * keystore.c - AES-256-GCM encrypted secret store (see keystore.h).
 *
 * On-disk format (all integers little-endian):
 *   magic      : 8 bytes  "SLKS0001"
 *   kdf_iters  : u32      PBKDF2 iteration count
 *   salt       : 16 bytes PBKDF2 salt
 *   file_nonce : 12 bytes GCM nonce for the whole payload
 *   ct_len     : u32      ciphertext length
 *   ciphertext : ct_len bytes
 *   tag        : 16 bytes GCM authentication tag
 *
 * The plaintext payload is the serialized record table:
 *   rec_count  : u32
 *   repeat rec_count times:
 *     name_len : u32, name bytes
 *     val_len  : u32, value bytes
 *
 * The whole payload is authenticated as one AEAD message, so any tampering
 * (or a wrong passphrase) is caught by the single GCM tag check on open.
 */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#  define _POSIX_C_SOURCE 200809L  /* strdup, fileno, etc. */
#endif

#include "keystore.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#if defined(_WIN32)
#  include <process.h>
#  define getpid_portable() ((unsigned)_getpid())
#else
#  include <unistd.h>
#  define getpid_portable() ((unsigned)getpid())
#endif

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/kdf.h>

#define KS_MAGIC      "SLKS0001"
#define KS_MAGIC_LEN  8
#define KS_SALT_LEN   16
#define KS_NONCE_LEN  12
#define KS_TAG_LEN    16
#define KS_KEY_LEN    32          /* AES-256 */
#define KS_DEFAULT_ITERS 600000u  /* OWASP-class PBKDF2-HMAC-SHA256 count */
#define KS_MAX_RECORDS   100000u
#define KS_MAX_FIELD     (64u * 1024u * 1024u)

typedef struct {
    char    *name;
    uint8_t *value;
    size_t   value_len;
} ks_record;

struct ks_store {
    char      *path;
    uint8_t    key[KS_KEY_LEN];
    uint8_t    salt[KS_SALT_LEN];
    uint32_t   iters;
    ks_record *records;
    size_t     count;
    size_t     capacity;
};

/* ---- small helpers -------------------------------------------------------*/

static void secure_zero(void *p, size_t n) {
    if (p) OPENSSL_cleanse(p, n);
}

static void put_u32(uint8_t *b, uint32_t v) {
    b[0] = (uint8_t)(v & 0xff);
    b[1] = (uint8_t)((v >> 8) & 0xff);
    b[2] = (uint8_t)((v >> 16) & 0xff);
    b[3] = (uint8_t)((v >> 24) & 0xff);
}

static uint32_t get_u32(const uint8_t *b) {
    return (uint32_t)b[0] | ((uint32_t)b[1] << 8) |
           ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
}

static ks_status derive_key(const char *passphrase,
                            const uint8_t *salt, uint32_t iters,
                            uint8_t key[KS_KEY_LEN]) {
    if (PKCS5_PBKDF2_HMAC(passphrase, (int)strlen(passphrase),
                          salt, KS_SALT_LEN, (int)iters,
                          EVP_sha256(), KS_KEY_LEN, key) != 1) {
        return KS_ERR_CRYPTO;
    }
    return KS_OK;
}

/* ---- record table --------------------------------------------------------*/

static ks_record *find_record(ks_store *ks, const char *name) {
    for (size_t i = 0; i < ks->count; ++i) {
        if (strcmp(ks->records[i].name, name) == 0) return &ks->records[i];
    }
    return NULL;
}

static void free_record(ks_record *r) {
    free(r->name);
    secure_zero(r->value, r->value_len);
    free(r->value);
    r->name = NULL;
    r->value = NULL;
    r->value_len = 0;
}

static ks_status ensure_capacity(ks_store *ks) {
    if (ks->count < ks->capacity) return KS_OK;
    size_t ncap = ks->capacity ? ks->capacity * 2 : 8;
    ks_record *nr = realloc(ks->records, ncap * sizeof(*nr));
    if (!nr) return KS_ERR_NOMEM;
    ks->records = nr;
    ks->capacity = ncap;
    return KS_OK;
}

/* ---- AEAD seal / open ----------------------------------------------------*/

static ks_status gcm_seal(const uint8_t key[KS_KEY_LEN],
                          const uint8_t nonce[KS_NONCE_LEN],
                          const uint8_t *pt, size_t pt_len,
                          uint8_t *ct, uint8_t tag[KS_TAG_LEN]) {
    EVP_CIPHER_CTX *c = EVP_CIPHER_CTX_new();
    if (!c) return KS_ERR_CRYPTO;
    ks_status rc = KS_ERR_CRYPTO;
    int outl = 0;
    if (EVP_EncryptInit_ex(c, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) goto done;
    if (EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_GCM_SET_IVLEN, KS_NONCE_LEN, NULL) != 1) goto done;
    if (EVP_EncryptInit_ex(c, NULL, NULL, key, nonce) != 1) goto done;
    if (pt_len > INT_MAX) goto done;
    if (EVP_EncryptUpdate(c, ct, &outl, pt, (int)pt_len) != 1) goto done;
    if (EVP_EncryptFinal_ex(c, ct + outl, &outl) != 1) goto done;
    if (EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_GCM_GET_TAG, KS_TAG_LEN, tag) != 1) goto done;
    rc = KS_OK;
done:
    EVP_CIPHER_CTX_free(c);
    return rc;
}

static ks_status gcm_open(const uint8_t key[KS_KEY_LEN],
                          const uint8_t nonce[KS_NONCE_LEN],
                          const uint8_t *ct, size_t ct_len,
                          const uint8_t tag[KS_TAG_LEN],
                          uint8_t *pt) {
    EVP_CIPHER_CTX *c = EVP_CIPHER_CTX_new();
    if (!c) return KS_ERR_CRYPTO;
    ks_status rc = KS_ERR_CRYPTO;
    int outl = 0;
    if (EVP_DecryptInit_ex(c, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) goto done;
    if (EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_GCM_SET_IVLEN, KS_NONCE_LEN, NULL) != 1) goto done;
    if (EVP_DecryptInit_ex(c, NULL, NULL, key, nonce) != 1) goto done;
    if (ct_len > INT_MAX) goto done;
    if (EVP_DecryptUpdate(c, pt, &outl, ct, (int)ct_len) != 1) goto done;
    if (EVP_CIPHER_CTX_ctrl(c, EVP_CTRL_GCM_SET_TAG, KS_TAG_LEN, (void *)tag) != 1) goto done;
    if (EVP_DecryptFinal_ex(c, pt + outl, &outl) != 1) { rc = KS_ERR_AUTH; goto done; }
    rc = KS_OK;
done:
    EVP_CIPHER_CTX_free(c);
    return rc;
}

/* ---- payload (de)serialization ------------------------------------------*/

static ks_status serialize_payload(ks_store *ks, uint8_t **out, size_t *out_len) {
    size_t total = 4; /* rec_count */
    for (size_t i = 0; i < ks->count; ++i) {
        total += 4 + strlen(ks->records[i].name) + 4 + ks->records[i].value_len;
    }
    uint8_t *buf = malloc(total ? total : 1);
    if (!buf) return KS_ERR_NOMEM;
    size_t off = 0;
    put_u32(buf + off, (uint32_t)ks->count); off += 4;
    for (size_t i = 0; i < ks->count; ++i) {
        size_t nl = strlen(ks->records[i].name);
        put_u32(buf + off, (uint32_t)nl); off += 4;
        memcpy(buf + off, ks->records[i].name, nl); off += nl;
        put_u32(buf + off, (uint32_t)ks->records[i].value_len); off += 4;
        memcpy(buf + off, ks->records[i].value, ks->records[i].value_len);
        off += ks->records[i].value_len;
    }
    *out = buf;
    *out_len = total;
    return KS_OK;
}

static ks_status deserialize_payload(ks_store *ks, const uint8_t *buf, size_t len) {
    size_t off = 0;
    if (len < 4) return KS_ERR_FORMAT;
    uint32_t n = get_u32(buf + off); off += 4;
    if (n > KS_MAX_RECORDS) return KS_ERR_FORMAT;
    for (uint32_t i = 0; i < n; ++i) {
        if (off + 4 > len) return KS_ERR_FORMAT;
        uint32_t nl = get_u32(buf + off); off += 4;
        if (nl > KS_MAX_FIELD || off + nl > len) return KS_ERR_FORMAT;
        char *name = malloc(nl + 1);
        if (!name) return KS_ERR_NOMEM;
        memcpy(name, buf + off, nl); name[nl] = '\0'; off += nl;
        if (off + 4 > len) { free(name); return KS_ERR_FORMAT; }
        uint32_t vl = get_u32(buf + off); off += 4;
        if (vl > KS_MAX_FIELD || off + vl > len) { free(name); return KS_ERR_FORMAT; }
        uint8_t *val = malloc(vl ? vl : 1);
        if (!val) { free(name); return KS_ERR_NOMEM; }
        memcpy(val, buf + off, vl); off += vl;
        if (ensure_capacity(ks) != KS_OK) { free(name); free(val); return KS_ERR_NOMEM; }
        ks->records[ks->count].name = name;
        ks->records[ks->count].value = val;
        ks->records[ks->count].value_len = vl;
        ks->count++;
    }
    return KS_OK;
}

/* ---- disk I/O ------------------------------------------------------------*/

static ks_status read_file(const char *path, uint8_t **out, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) return KS_ERR_IO;
    if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return KS_ERR_IO; }
    long sz = ftell(f);
    if (sz < 0) { fclose(f); return KS_ERR_IO; }
    rewind(f);
    uint8_t *buf = malloc((size_t)sz ? (size_t)sz : 1);
    if (!buf) { fclose(f); return KS_ERR_NOMEM; }
    if (sz > 0 && fread(buf, 1, (size_t)sz, f) != (size_t)sz) {
        free(buf); fclose(f); return KS_ERR_IO;
    }
    fclose(f);
    *out = buf; *out_len = (size_t)sz;
    return KS_OK;
}

/* Serialize + encrypt the whole store and write it atomically via rename. */
static ks_status persist(ks_store *ks) {
    uint8_t *payload = NULL; size_t payload_len = 0;
    ks_status rc = serialize_payload(ks, &payload, &payload_len);
    if (rc != KS_OK) return rc;

    uint8_t nonce[KS_NONCE_LEN], tag[KS_TAG_LEN];
    if (RAND_bytes(nonce, KS_NONCE_LEN) != 1) {
        secure_zero(payload, payload_len); free(payload); return KS_ERR_CRYPTO;
    }
    uint8_t *ct = malloc(payload_len ? payload_len : 1);
    if (!ct) { secure_zero(payload, payload_len); free(payload); return KS_ERR_NOMEM; }

    rc = gcm_seal(ks->key, nonce, payload, payload_len, ct, tag);
    secure_zero(payload, payload_len); free(payload);
    if (rc != KS_OK) { free(ct); return rc; }

    /* Build header+body. */
    size_t hdr = KS_MAGIC_LEN + 4 + KS_SALT_LEN + KS_NONCE_LEN + 4;
    size_t total = hdr + payload_len + KS_TAG_LEN;
    uint8_t *out = malloc(total);
    if (!out) { free(ct); return KS_ERR_NOMEM; }
    size_t off = 0;
    memcpy(out + off, KS_MAGIC, KS_MAGIC_LEN); off += KS_MAGIC_LEN;
    put_u32(out + off, ks->iters); off += 4;
    memcpy(out + off, ks->salt, KS_SALT_LEN); off += KS_SALT_LEN;
    memcpy(out + off, nonce, KS_NONCE_LEN); off += KS_NONCE_LEN;
    put_u32(out + off, (uint32_t)payload_len); off += 4;
    memcpy(out + off, ct, payload_len); off += payload_len;
    memcpy(out + off, tag, KS_TAG_LEN); off += KS_TAG_LEN;
    free(ct);

    /* Atomic write: temp file in same dir, then rename(2). */
    size_t tmplen = strlen(ks->path) + 12;
    char *tmp = malloc(tmplen);
    if (!tmp) { free(out); return KS_ERR_NOMEM; }
    snprintf(tmp, tmplen, "%s.tmp%u", ks->path, (unsigned)getpid_portable());
    FILE *f = fopen(tmp, "wb");
    if (!f) { free(out); free(tmp); return KS_ERR_IO; }
    int ok = (fwrite(out, 1, total, f) == total);
    fflush(f);
    fclose(f);
    free(out);
    if (!ok) { remove(tmp); free(tmp); return KS_ERR_IO; }
    if (rename(tmp, ks->path) != 0) { remove(tmp); free(tmp); return KS_ERR_IO; }
    free(tmp);
    return KS_OK;
}

/* ---- public API ----------------------------------------------------------*/

ks_status ks_open(const char *path, const char *passphrase, ks_store **out) {
    if (!path || !passphrase || !out) return KS_ERR_ARG;
    *out = NULL;

    ks_store *ks = calloc(1, sizeof(*ks));
    if (!ks) return KS_ERR_NOMEM;
    ks->path = strdup(path);
    if (!ks->path) { free(ks); return KS_ERR_NOMEM; }
    ks->iters = KS_DEFAULT_ITERS;

    uint8_t *raw = NULL; size_t raw_len = 0;
    ks_status io = read_file(path, &raw, &raw_len);

    if (io == KS_ERR_IO) {
        /* New store: fresh random salt, derive key, persist an empty store. */
        if (RAND_bytes(ks->salt, KS_SALT_LEN) != 1) { ks_close(ks); return KS_ERR_CRYPTO; }
        ks_status rc = derive_key(passphrase, ks->salt, ks->iters, ks->key);
        if (rc != KS_OK) { ks_close(ks); return rc; }
        rc = persist(ks);
        if (rc != KS_OK) { ks_close(ks); return rc; }
        *out = ks;
        return KS_OK;
    }
    if (io != KS_OK) { ks_close(ks); return io; }

    /* Existing store: parse header, derive key, authenticate+decrypt. */
    size_t minlen = KS_MAGIC_LEN + 4 + KS_SALT_LEN + KS_NONCE_LEN + 4 + KS_TAG_LEN;
    if (raw_len < minlen || memcmp(raw, KS_MAGIC, KS_MAGIC_LEN) != 0) {
        free(raw); ks_close(ks); return KS_ERR_FORMAT;
    }
    size_t off = KS_MAGIC_LEN;
    ks->iters = get_u32(raw + off); off += 4;
    if (ks->iters == 0 || ks->iters > 100000000u) { free(raw); ks_close(ks); return KS_ERR_FORMAT; }
    memcpy(ks->salt, raw + off, KS_SALT_LEN); off += KS_SALT_LEN;
    uint8_t nonce[KS_NONCE_LEN];
    memcpy(nonce, raw + off, KS_NONCE_LEN); off += KS_NONCE_LEN;
    uint32_t ct_len = get_u32(raw + off); off += 4;
    if ((size_t)ct_len + KS_TAG_LEN + off != raw_len) { free(raw); ks_close(ks); return KS_ERR_FORMAT; }
    const uint8_t *ct = raw + off;
    const uint8_t *tag = raw + off + ct_len;

    ks_status rc = derive_key(passphrase, ks->salt, ks->iters, ks->key);
    if (rc != KS_OK) { free(raw); ks_close(ks); return rc; }

    uint8_t *pt = malloc(ct_len ? ct_len : 1);
    if (!pt) { free(raw); ks_close(ks); return KS_ERR_NOMEM; }
    rc = gcm_open(ks->key, nonce, ct, ct_len, tag, pt);
    if (rc != KS_OK) { secure_zero(pt, ct_len); free(pt); free(raw); ks_close(ks); return rc; }

    rc = deserialize_payload(ks, pt, ct_len);
    secure_zero(pt, ct_len); free(pt); free(raw);
    if (rc != KS_OK) { ks_close(ks); return rc; }

    *out = ks;
    return KS_OK;
}

ks_status ks_put(ks_store *ks, const char *name,
                 const uint8_t *value, size_t value_len) {
    if (!ks || !name || (!value && value_len)) return KS_ERR_ARG;
    if (strlen(name) == 0 || strlen(name) > KS_MAX_FIELD) return KS_ERR_ARG;
    if (value_len > KS_MAX_FIELD) return KS_ERR_ARG;

    ks_record *r = find_record(ks, name);
    if (r) {
        /* Idempotent replace. */
        if (r->value_len == value_len &&
            (value_len == 0 || memcmp(r->value, value, value_len) == 0)) {
            return KS_OK; /* unchanged - no rewrite needed */
        }
        uint8_t *nv = malloc(value_len ? value_len : 1);
        if (!nv) return KS_ERR_NOMEM;
        if (value_len) memcpy(nv, value, value_len);
        secure_zero(r->value, r->value_len);
        free(r->value);
        r->value = nv;
        r->value_len = value_len;
        return persist(ks);
    }

    if (ensure_capacity(ks) != KS_OK) return KS_ERR_NOMEM;
    char *nm = strdup(name);
    if (!nm) return KS_ERR_NOMEM;
    uint8_t *nv = malloc(value_len ? value_len : 1);
    if (!nv) { free(nm); return KS_ERR_NOMEM; }
    if (value_len) memcpy(nv, value, value_len);
    ks->records[ks->count].name = nm;
    ks->records[ks->count].value = nv;
    ks->records[ks->count].value_len = value_len;
    ks->count++;
    return persist(ks);
}

ks_status ks_put_str(ks_store *ks, const char *name, const char *value) {
    if (!value) return KS_ERR_ARG;
    return ks_put(ks, name, (const uint8_t *)value, strlen(value));
}

ks_status ks_get(ks_store *ks, const char *name,
                 uint8_t **out_value, size_t *out_len) {
    if (!ks || !name || !out_value || !out_len) return KS_ERR_ARG;
    ks_record *r = find_record(ks, name);
    if (!r) return KS_ERR_NOTFOUND;
    uint8_t *v = malloc(r->value_len ? r->value_len : 1);
    if (!v) return KS_ERR_NOMEM;
    if (r->value_len) memcpy(v, r->value, r->value_len);
    *out_value = v;
    *out_len = r->value_len;
    return KS_OK;
}

ks_status ks_remove(ks_store *ks, const char *name) {
    if (!ks || !name) return KS_ERR_ARG;
    for (size_t i = 0; i < ks->count; ++i) {
        if (strcmp(ks->records[i].name, name) == 0) {
            free_record(&ks->records[i]);
            ks->records[i] = ks->records[ks->count - 1];
            ks->count--;
            return persist(ks);
        }
    }
    return KS_OK; /* idempotent: absent name is success */
}

size_t ks_count(const ks_store *ks) { return ks ? ks->count : 0; }

int ks_list(const ks_store *ks, char **names, size_t max) {
    if (!ks || (!names && max)) return KS_ERR_ARG;
    size_t n = ks->count < max ? ks->count : max;
    for (size_t i = 0; i < n; ++i) {
        names[i] = strdup(ks->records[i].name);
        if (!names[i]) {
            for (size_t j = 0; j < i; ++j) free(names[j]);
            return KS_ERR_NOMEM;
        }
    }
    return (int)n;
}

void ks_free_secret(uint8_t *value, size_t len) {
    if (value) { secure_zero(value, len); free(value); }
}

void ks_close(ks_store *ks) {
    if (!ks) return;
    for (size_t i = 0; i < ks->count; ++i) free_record(&ks->records[i]);
    free(ks->records);
    secure_zero(ks->key, KS_KEY_LEN);
    free(ks->path);
    free(ks);
}

const char *ks_strerror(ks_status st) {
    switch (st) {
        case KS_OK:           return "ok";
        case KS_ERR_IO:       return "I/O error";
        case KS_ERR_CRYPTO:   return "cryptographic operation failed";
        case KS_ERR_AUTH:     return "authentication failed (wrong passphrase or tampered store)";
        case KS_ERR_FORMAT:   return "unrecognized or corrupt store format";
        case KS_ERR_NOTFOUND: return "no such record";
        case KS_ERR_ARG:      return "invalid argument";
        case KS_ERR_NOMEM:    return "out of memory";
    }
    return "unknown error";
}
