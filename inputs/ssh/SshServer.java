// SshServer.java -- a model of the SSH transport/auth handshake, as a toolchain
// input. It does NOT implement SSH cryptography (that requires a real crypto
// stack); instead it models the message sequence of RFC 4253/4252 -- version
// exchange, KEXINIT/NEWKEYS, and a userauth loop that accepts "publickey" and
// rejects "none"/"password" -- mirroring SshServer.sleela in this directory.
//
//   javac SshServer.java && java SshServer
package inputs.ssh;

public final class SshServer {

    private static final int MAX_TRIES = 3;

    private static boolean authMethodAccepted(String method) {
        return method.equals("publickey");
    }

    /** Returns true when authentication succeeds within the attempt budget. */
    private static boolean userauth(String method, int attempt) {
        if (attempt > MAX_TRIES) {
            System.out.println("SSH_MSG_DISCONNECT: too many auth failures");
            return false;
        }
        if (authMethodAccepted(method)) {
            System.out.println("SSH_MSG_USERAUTH_SUCCESS (method=" + method + ")");
            return true;
        }
        System.out.println(
                "SSH_MSG_USERAUTH_FAILURE (method=" + method + ", attempt=" + attempt + ")");
        return false;
    }

    public static void main(String[] args) {
        System.out.println("== Java SSH server model: transport + auth handshake ==");

        // 1) Protocol version exchange (RFC 4253 sec 4.2)
        System.out.println("S: SSH-2.0-SleelaModel_0.1");

        // 2) Key exchange banner (algorithms negotiated here in a real server)
        System.out.println("S: SSH_MSG_KEXINIT (kex=curve25519, host-key=ed25519)");
        System.out.println("S: SSH_MSG_NEWKEYS");

        // 3) Authentication loop: client offers none, then password, then publickey.
        String[] offered = {"none", "password", "publickey"};
        boolean ok = false;
        int attempt = 1;
        for (String method : offered) {
            ok = userauth(method, attempt);
            attempt++;
            if (ok) break;
        }

        // 4) Channel open only if authenticated.
        if (ok) {
            System.out.println("S: SSH_MSG_CHANNEL_OPEN_CONFIRMATION (session)");
            System.out.println("S: shell channel ready");
        } else {
            System.out.println("S: connection refused");
        }
        System.out.println("== handshake complete ==");
    }
}
