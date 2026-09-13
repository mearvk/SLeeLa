package com.mearvk.sleela.java28;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

/**
 * Port channel (J28-MEM-0001 §4): a TCP loopback server exposing the Java 28
 * SecureJDK memory model. Each accepted connection is a session whose object
 * memory is owned by a fresh {@link SleelaMemoryServer}. The Sleela-side driver
 * (native client) connects, drives objects by handle, and disconnects.
 *
 * <p>Usage: {@code java ... Java28PortServer [port]}. With port 0 (or absent)
 * an ephemeral port is chosen and printed as {@code PORT <n>} on stdout so a
 * launcher can hand it to the driver. Bound to 127.0.0.1 only.
 */
public final class Java28PortServer {

    private final ServerSocket server;

    public Java28PortServer(int port) throws Exception {
        this.server = new ServerSocket(port, 16, InetAddress.getByName("127.0.0.1"));
    }

    public int port() { return server.getLocalPort(); }

    /** Build a memory host with the demo allow-list registered. */
    static SleelaMemoryServer newHost() {
        SleelaMemoryServer host = new SleelaMemoryServer();
        host.registerClass(Counter.class);
        host.registerClass(Ledger.class);
        return host;
    }

    /** Serve a single connection, then return (demo-friendly, one client). */
    public void serveOne() throws Exception {
        try (Socket s = server.accept()) {
            SleelaMemoryServer host = newHost();
            BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream(), StandardCharsets.UTF_8));
            BufferedWriter out = new BufferedWriter(new OutputStreamWriter(s.getOutputStream(), StandardCharsets.UTF_8));
            String line;
            while ((line = in.readLine()) != null) {
                String reply = host.handleLine(line);
                out.write(reply);
                out.write('\n');
                out.flush();
                if (line.strip().equals("bye")) break;
            }
        }
    }

    public void close() throws Exception { server.close(); }

    public static void main(String[] args) throws Exception {
        int requested = args.length > 0 ? Integer.parseInt(args[0]) : 0;
        Java28PortServer srv = new Java28PortServer(requested);
        System.out.println("PORT " + srv.port());
        System.out.flush();
        try {
            srv.serveOne();
        } finally {
            srv.close();
        }
    }
}
