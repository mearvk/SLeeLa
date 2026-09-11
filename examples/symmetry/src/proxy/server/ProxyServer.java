package symmetry.proxy.server;

import java.util.concurrent.atomic.AtomicLong;

public final class ProxyServer {
    private final AtomicLong activeConnections = new AtomicLong();
    private final AtomicLong forwardedRequests = new AtomicLong();
    public void connect() { activeConnections.incrementAndGet(); }
    public String forward(String target, String payload) { forwardedRequests.incrementAndGet(); return payload; }
    public void disconnect() { activeConnections.decrementAndGet(); }
    public long getActiveConnections() { return activeConnections.get(); }
    public long getForwardedRequests() { return forwardedRequests.get(); }
}
