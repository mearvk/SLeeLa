// FtpServer.java -- a model FTP control-channel server (RFC 959), as a
// toolchain input. It speaks the FTP control protocol over a real TCP socket:
// it sends 3-digit reply codes and handles USER/PASS/PWD/LIST/QUIT. Data
// connections are intentionally not implemented -- this focuses on the control
// channel state machine, mirroring FtpServer.sleela in this directory.
//
//   javac FtpServer.java && java FtpServer 2121
//   (then: nc localhost 2121  -> type USER anonymous / PASS x / PWD / QUIT)
package inputs.ftp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public final class FtpServer {

    public static void main(String[] args) throws IOException {
        int port = args.length > 0 ? Integer.parseInt(args[0]) : 2121;
        try (ServerSocket listener = new ServerSocket(port)) {
            System.out.println("FTP control server listening on port " + port);
            while (true) {
                try (Socket client = listener.accept()) {
                    handleSession(client);
                } catch (IOException e) {
                    System.err.println("session error: " + e.getMessage());
                }
            }
        }
    }

    private static void handleSession(Socket client) throws IOException {
        BufferedReader in = new BufferedReader(
                new InputStreamReader(client.getInputStream(), StandardCharsets.UTF_8));
        PrintWriter out = new PrintWriter(client.getOutputStream(), true);

        boolean loggedIn = false;
        reply(out, 220, "Service ready");

        String line;
        while ((line = in.readLine()) != null) {
            String[] parts = line.trim().split("\\s+", 2);
            String verb = parts[0].toUpperCase();
            switch (verb) {
                case "USER":
                    reply(out, 331, "User name okay, need password");
                    break;
                case "PASS":
                    loggedIn = true; // model: any PASS after USER authenticates
                    reply(out, 230, "User logged in, proceed");
                    break;
                case "PWD":
                    if (loggedIn) reply(out, 257, "\"/\" is current directory");
                    else reply(out, 530, "Not logged in");
                    break;
                case "LIST":
                    if (loggedIn) reply(out, 226, "Directory send OK");
                    else reply(out, 530, "Not logged in");
                    break;
                case "QUIT":
                    reply(out, 221, "Goodbye");
                    return;
                default:
                    reply(out, 502, "Command not implemented");
            }
        }
    }

    private static void reply(PrintWriter out, int code, String text) {
        out.print(code + " " + text + "\r\n");
        out.flush();
        System.out.println("S: " + code + " " + text);
    }
}
