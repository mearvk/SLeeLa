package symmetry.proxy.client;

public final class ProxyClient {
    private String proxy;
    private long requestCount;
    public void connect(String proxy) { this.proxy = proxy; }
    public String request(String target, String payload) { requestCount++; return payload; }
    public String getProxy() { return proxy; }
    public long getRequestCount() { return requestCount; }
}
