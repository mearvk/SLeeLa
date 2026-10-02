<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">





# AE6E66 Operational Scripts

- `verify-integrity.sh` — fail-closed SHA-256 verifier.
- `install-postfix-dovecot.sh` — non-destructive MTA preflight.
- `setup-dkim.sh` — non-destructive DKIM/MTA preflight.
- `setup-mysql.sh` — non-destructive database security preflight.

These scripts intentionally do not install packages, generate private keys, create credentials, change DNS/firewall state, or enable services. Host administration is explicit and external to AE6E66.