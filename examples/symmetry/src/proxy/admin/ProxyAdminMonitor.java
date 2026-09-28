package symmetry.proxy.admin;

import symmetry.proxy.server.ProxyServer;

public final class ProxyAdminMonitor {
    private final ProxyServer server;
    public ProxyAdminMonitor(ProxyServer server) { this.server = server; }
    public String inspect() { return "connections=" + server.getActiveConnections() + ", forwarded=" + server.getForwardedRequests(); }
    public void drain() { while (server.getActiveConnections() > 0) server.disconnect(); }
}
