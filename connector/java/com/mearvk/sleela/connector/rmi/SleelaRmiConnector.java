package com.mearvk.sleela.connector.rmi;

import com.mearvk.sleela.connector.SleelaInvocation;
import com.mearvk.sleela.connector.SleelaJavaConnector;
import com.mearvk.sleela.connector.SleelaResult;
import com.mearvk.sleela.rmi.SleelaRmiClient;

import java.util.Objects;

/** Common-connector adapter over the existing Java RMI SLeeLa service. */
public final class SleelaRmiConnector implements SleelaJavaConnector {
    private final SleelaRmiClient client;

    public SleelaRmiConnector(String host, int registryPort, String serviceName) throws Exception {
        this.client = new SleelaRmiClient(host, registryPort, serviceName);
    }

    public SleelaRmiConnector(SleelaRmiClient client) {
        this.client = Objects.requireNonNull(client, "client");
    }

    @Override
    public SleelaResult invoke(SleelaInvocation invocation) {
        try {
            return SleelaResult.success(client.invoke(invocation.operation(), invocation.arguments()));
        } catch (Exception failure) {
            return SleelaResult.failure(messageOf(failure));
        }
    }

    @Override
    public String health() throws Exception {
        return client.health();
    }

    @Override
    public void close() {
        client.close();
    }

    private static String messageOf(Throwable failure) {
        return failure.getMessage() == null ? failure.toString() : failure.getMessage();
    }
}
