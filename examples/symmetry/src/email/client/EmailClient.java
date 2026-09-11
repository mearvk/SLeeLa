package symmetry.email.client;

public final class EmailClient {
    private String server;
    private long sentCount;
    public void connect(String server) { this.server = server; }
    public void send(String message) { sentCount++; }
    public String fetch() { return null; }
    public String getServer() { return server; }
    public long getSentCount() { return sentCount; }
}
