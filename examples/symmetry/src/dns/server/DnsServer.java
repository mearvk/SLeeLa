package symmetry.dns.server;

public final class DnsServer {
    private boolean listening;
    private long requestCount;

    public void start() { listening = true; requestCount = 0; }
    public String query(String name) {
        requestCount++;
        return resolve(name);
    }
    private String resolve(String name) { return name; }
    public void stop() { listening = false; }
    public boolean isListening() { return listening; }
    public long getRequestCount() { return requestCount; }
}
