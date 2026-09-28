package com.mearvk.sleela.synchro;
import java.net.InetSocketAddress;
public final class SynchroIntegrationTest {
    public static void main(String[] args) throws Exception {
        SynchroIntegration integration = new SynchroIntegration(
            new InetSocketAddress[]{new InetSocketAddress("127.0.0.1", 9)},
            16, 25, 8);
        if (integration.evaluate(1000.0, 95.0) == null)
            throw new AssertionError("SLA result must not be null");
        System.out.println("synchro Java integration: PASS");
    }
}
