#!/usr/bin/env bash
# test.sh - Safety + functional battery for the advantage transport layer.
#
# Exercises, without requiring public-internet egress:
#   * DH/ECDH key exchange (all groups) produce matching session keys
#   * keystore round-trip, idempotent replace, wrong-passphrase rejection,
#     and that secrets are absent from the on-disk ciphertext
#   * acquisition idempotency + content pinning (via file://)
#   * bulk fetcher over a file:// list (parallel + idempotent)
#   * HTTPS TLS verification: default rejects a self-signed cert; with
#     --insecure the transfer succeeds (handshake + download + store)
#
# Exit code is non-zero if any check fails.
set -u

HERE="$(cd "$(dirname "$0")" && pwd)"
T="$HERE/transport"
BULK="$HERE/bulkfetch"
WORK="$(mktemp -d "${TMPDIR:-/tmp}/advtest.XXXXXX")"
trap 'rm -rf "$WORK"' EXIT

pass=0; fail=0
ok()   { echo "  PASS: $1"; pass=$((pass+1)); }
bad()  { echo "  FAIL: $1"; fail=$((fail+1)); }
chk()  { if eval "$2"; then ok "$1"; else bad "$1"; fi; }

echo "== build =="
make -C "$HERE" all >/dev/null 2>&1 || { echo "build failed"; exit 1; }
[ -x "$T" ] && [ -x "$BULK" ] || { echo "binaries missing"; exit 1; }
echo "  built transport + bulkfetch"

echo "== 1. key exchange (session keys must match) =="
for g in x25519 p256 ffdhe2048 ffdhe3072; do
  out="$("$T" kex "$g" 2>&1)"
  if echo "$out" | grep -q "match: yes"; then ok "kex $g"; else bad "kex $g"; fi
done

echo "== 2. keystore =="
KS="$WORK/ks.bin"
PASS='correct horse battery staple'
ADVANTAGE_KS_PASS="$PASS" "$T" keystore "$KS" put token 'secret-ABC-123' >/dev/null 2>&1
val="$(ADVANTAGE_KS_PASS="$PASS" "$T" keystore "$KS" get token 2>/dev/null)"
chk "keystore round-trip" '[ "$val" = "secret-ABC-123" ]'
# idempotent replace (same value) should still succeed
ADVANTAGE_KS_PASS="$PASS" "$T" keystore "$KS" put token 'secret-ABC-123' >/dev/null 2>&1
chk "keystore idempotent put" '[ $? -eq 0 ]'
# wrong passphrase must fail
if ADVANTAGE_KS_PASS="wrong" "$T" keystore "$KS" get token >/dev/null 2>&1; then
  bad "keystore wrong-passphrase rejected"
else
  ok "keystore wrong-passphrase rejected"
fi
# secret not present in plaintext on disk
if strings "$KS" | grep -q "secret-ABC-123"; then
  bad "keystore encrypted at rest"
else
  ok "keystore encrypted at rest"
fi

echo "== 3. acquisition idempotency + pinning (file://) =="
SRC="$WORK/src.txt"; printf 'content-addressed and idempotent\n' > "$SRC"
SHA="$(sha256sum "$SRC" | cut -d' ' -f1)"
ST="$WORK/store"
out1="$("$T" fetch "file://$SRC" --store "$ST" 2>&1)"
chk "first fetch stores"    'echo "$out1" | grep -q "^stored "'
out2="$("$T" fetch "file://$SRC" --store "$ST" 2>&1)"
chk "second fetch cached"   'echo "$out2" | grep -q "^cached "'
out3="$("$T" fetch "file://$SRC" --store "$ST" --sha256 "$SHA" 2>&1)"
chk "correct sha pin passes" 'echo "$out3" | grep -qi "cached\|stored"'
if "$T" fetch "file://$SRC" --store "$ST" --sha256 0000000000000000000000000000000000000000000000000000000000000000 >/dev/null 2>&1; then
  bad "wrong sha pin rejected"
else
  ok "wrong sha pin rejected"
fi

echo "== 4. bulk fetcher (parallel + idempotent, file://) =="
LIST="$WORK/list.txt"
printf 'file://%s\nfile:///etc/hostname\n' "$SRC" > "$LIST"
b1="$("$BULK" "$LIST" --store "$WORK/bulkstore" --jobs 4 2>&1)"
chk "bulk first run all ok"  'echo "$b1" | grep -q "failed 0"'
b2="$("$BULK" "$LIST" --store "$WORK/bulkstore" --jobs 4 2>&1)"
chk "bulk second run cached" 'echo "$b2" | grep -qE "cached [1-9]"'

echo "== 5. HTTPS TLS verification (local self-signed server) =="
if command -v python3 >/dev/null 2>&1; then
  # Generate a self-signed localhost cert via a tiny OpenSSL C helper.
  GC="$WORK/gencert.c"
  cat > "$GC" <<'C'
#include <openssl/evp.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <stdio.h>
int main(int argc,char**argv){
  EVP_PKEY*pkey=EVP_RSA_gen(2048); if(!pkey)return 1;
  X509*x=X509_new(); ASN1_INTEGER_set(X509_get_serialNumber(x),1);
  X509_gmtime_adj(X509_getm_notBefore(x),0); X509_gmtime_adj(X509_getm_notAfter(x),86400L);
  X509_set_pubkey(x,pkey);
  X509_NAME*n=X509_get_subject_name(x);
  X509_NAME_add_entry_by_txt(n,"CN",MBSTRING_ASC,(unsigned char*)"localhost",-1,-1,0);
  X509_set_issuer_name(x,n);
  X509V3_CTX c; X509V3_set_ctx_nodb(&c); X509V3_set_ctx(&c,x,x,NULL,NULL,0);
  X509_EXTENSION*e=X509V3_EXT_conf_nid(NULL,&c,NID_subject_alt_name,"DNS:localhost,IP:127.0.0.1");
  if(e){X509_add_ext(x,e,-1);X509_EXTENSION_free(e);}
  X509_sign(x,pkey,EVP_sha256());
  FILE*k=fopen(argv[1],"wb");PEM_write_PrivateKey(k,pkey,NULL,NULL,0,NULL,NULL);fclose(k);
  FILE*cc=fopen(argv[2],"wb");PEM_write_X509(cc,x);fclose(cc);
  return 0;
}
C
  OSSL_CF="$(pkg-config --cflags --libs openssl 2>/dev/null || echo -lssl -lcrypto)"
  if cc "$GC" -o "$WORK/gencert" $OSSL_CF 2>/dev/null && "$WORK/gencert" "$WORK/key.pem" "$WORK/cert.pem"; then
    mkdir -p "$WORK/webroot"; printf 'tls ok\n' > "$WORK/webroot/hello.txt"
    cat > "$WORK/driver.py" <<PY
import http.server, ssl, os, threading, subprocess, time, sys
os.chdir("$WORK/webroot")
ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
ctx.load_cert_chain("$WORK/cert.pem", "$WORK/key.pem")
httpd = http.server.ThreadingHTTPServer(("127.0.0.1", 0), http.server.SimpleHTTPRequestHandler)
port = httpd.socket.getsockname()[1]
httpd.socket = ctx.wrap_socket(httpd.socket, server_side=True)
threading.Thread(target=httpd.serve_forever, daemon=True).start()
time.sleep(0.4)
url = f"https://localhost:{port}/hello.txt"
def run(args):
    return subprocess.run(["$T"]+args, capture_output=True, text=True)
r1 = run(["fetch", url, "--store", "$WORK/tls1"])
r2 = run(["fetch", url, "--store", "$WORK/tls2", "--insecure"])
httpd.shutdown()
rej = ("verify failed" in (r1.stdout+r1.stderr)) or (r1.returncode != 0)
got = ("stored" in r2.stdout) or ("cached" in r2.stdout)
print("REJECT_SELFSIGNED", "yes" if rej else "no")
print("INSECURE_FETCH", "yes" if got else "no")
PY
    tls_out="$(timeout 40 python3 "$WORK/driver.py" 2>&1)"
    chk "default verify rejects self-signed cert" 'echo "$tls_out" | grep -q "REJECT_SELFSIGNED yes"'
    chk "insecure fetch downloads + stores"       'echo "$tls_out" | grep -q "INSECURE_FETCH yes"'
  else
    echo "  SKIP: could not build cert helper"
  fi
else
  echo "  SKIP: python3 not available for local TLS server"
fi

echo "============================================"
echo "RESULT: $pass passed, $fail failed"
[ "$fail" -eq 0 ]
