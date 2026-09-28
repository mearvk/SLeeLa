package symmetry.dns.admin;

import symmetry.dns.server.DnsServer;

public final class DnsAdminMonitor {
    private final DnsServer server;
    public DnsAdminMonitor(DnsServer server) { this.server = server; }
    public String inspect() { return "listening=" + server.isListening() + ", requests=" + server.getRequestCount(); }
    public void reload() { if (!server.isListening()) server.start(); }
}
