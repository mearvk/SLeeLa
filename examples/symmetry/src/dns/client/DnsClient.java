package symmetry.dns.client;

public final class DnsClient {
    private String resolver;
    private long queryCount;

    public void start(String resolver) { this.resolver = resolver; queryCount = 0; }
    public String lookup(String name) { queryCount++; return name; }
    public String getResolver() { return resolver; }
    public long getQueryCount() { return queryCount; }
}
