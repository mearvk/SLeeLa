package com.mearvk.sleela.rmi;

import com.mearvk.sleela.gui.ProcessSleelaRuntime;

import java.nio.file.Path;

/** Executable server bootstrap for a SLeeLa service over Java RMI/JRMP. */
public final class SleelaRmiServer {
    private SleelaRmiServer() {
    }

    public static void main(String[] args) throws Exception {
        if (args.length < 3 || args.length > 4) {
            System.err.println("usage: SleelaRmiServer <service-name> <sleela-executable> <working-directory> [registry-port]");
            System.exit(2);
        }

        String serviceName = args[0];
        Path executable = Path.of(args[1]);
        Path workingDirectory = Path.of(args[2]);
        int registryPort = args.length == 4 ? Integer.parseInt(args[3]) : 1099;

        ProcessSleelaRuntime runtime = new ProcessSleelaRuntime(executable, workingDirectory);
        SleelaRmiServerHandle server = SleelaRmiServerHandle.start(serviceName, registryPort, runtime);

        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            server.close();
            runtime.close();
        }, "sleela-rmi-shutdown"));

        System.out.println("SLeeLa RMI server started");
        System.out.println("service=" + server.serviceName());
        System.out.println("registry=127.0.0.1:" + server.registryPort());
        System.out.println(server.health());

        Thread.currentThread().join();
    }
}
