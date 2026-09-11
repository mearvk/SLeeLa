package symmetry.email.admin;

import symmetry.email.server.EmailServer;

public final class EmailAdminMonitor {
    private final EmailServer server;
    public EmailAdminMonitor(EmailServer server) { this.server = server; }
    public String inspect() { return "queue=" + server.getQueueSize() + ", delivered=" + server.getDeliveredCount(); }
}
