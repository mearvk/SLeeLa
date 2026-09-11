# `inputs/` — server programs in Sleela, Java, and C

This directory holds small **server programs** written in each of the three
Sleela toolchain targets — **Sleela** (`.sleela`), **Java** (`.java`), and
**C** (`.c`) — one triplet per protocol:

| Protocol | Sleela                    | Java                    | C                       |
|----------|---------------------------|-------------------------|-------------------------|
| HTTP     | `http/HttpServer.sleela`  | `http/HttpServer.java`  | `http/http_server.c`    |
| FTP      | `ftp/FtpServer.sleela`    | `ftp/FtpServer.java`    | `ftp/ftp_server.c`      |
| SSH      | `ssh/SshServer.sleela`    | `ssh/SshServer.java`    | `ssh/ssh_server.c`      |

They are intended as **toolchain inputs** — Sleela source files are *Wrapper™*
program units, and the Java/C files show what the corresponding transpiler
target would look like for the same protocol logic.

## What they model (and what they don't)

Each triplet implements the same protocol *shape* so the three files line up
line-for-line in behavior:

- **HTTP** — an HTTP/1.1 request handler: method/path routing to status codes
  (`200`, `404` for unknown paths, `405` for unsupported methods) and a small
  response body.
- **FTP** — the RFC 959 **control channel** state machine: 3-digit reply codes
  for `USER` / `PASS` / `PWD` / `LIST` / `QUIT`, with a "logged in" gate that
  returns `530` before authentication. Data connections are not implemented.
- **SSH** — the RFC 4253/4252 **handshake sequence**: version-string exchange,
  `KEXINIT` / `NEWKEYS` banners, and a `userauth` loop that accepts `publickey`
  and rejects `none` / `password`.

**Scope note.** These are protocol-shaped **models/demos**, not production
network daemons:

- The **Java** and **C** HTTP and FTP servers open **real TCP sockets** and
  speak the wire protocol (you can `curl` / `nc` them).
- The **SSH** programs model the *message sequence only* — there is **no
  cryptography** (no key exchange math, no ciphers). Implementing real SSH
  requires a vetted crypto library; that is deliberately out of scope for an
  input sample.
- The **Sleela** versions simulate each protocol **in-language** by feeding a
  fixed batch of requests/commands through the same routing/status logic, since
  the Sleela front end today has no sockets or arrays (see
  [`../impl/README.md`](../impl/README.md) for the supported feature set).

## Building and running

### Sleela (`.sleela`)

Build the Sleela toolchain once, then run each Wrapper™:

```sh
cd ../impl && make
./build/sleela run ../inputs/http/HttpServer.sleela
./build/sleela run ../inputs/ftp/FtpServer.sleela
./build/sleela run ../inputs/ssh/SshServer.sleela
```

### Java (JDK 17+; verified on JDK 25)

The files declare `package inputs.<proto>;`, so compile from this directory:

```sh
javac http/HttpServer.java ftp/FtpServer.java ssh/SshServer.java -d out
java -cp out inputs.ssh.SshServer                 # prints the handshake
java -cp out inputs.http.HttpServer 8080          # real server; curl -i localhost:8080/health
java -cp out inputs.ftp.FtpServer  2121           # real server; nc localhost 2121
```

### C (C11; POSIX sockets)

```sh
cc -std=c11 -O2 -Wall -Wextra ssh/ssh_server.c  -o ssh_server  && ./ssh_server
cc -std=c11 -O2 -Wall -Wextra http/http_server.c -o http_server && ./http_server 8080
cc -std=c11 -O2 -Wall -Wextra ftp/ftp_server.c  -o ftp_server  && ./ftp_server 2121
```

## Verification status

All nine files were compiled and exercised when added:

- **Sleela** — all three `check` clean and `run` on the C core.
- **Java** — all three compile under JDK 25; the HTTP server was confirmed
  returning `200` / `404` / `405` over HTTP.
- **C** — all three compile clean under `-Wall -Wextra`; the HTTP server was
  confirmed over `curl`, and the FTP server over a full control-channel session.
