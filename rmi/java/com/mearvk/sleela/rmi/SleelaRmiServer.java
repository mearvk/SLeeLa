package com.mearvk.sleela.rmi;

import com.mearvk.sleela.gui.ProcessSleelaRuntime;

import java.nio.file.Path;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

/**
 * Standard Java RMI/JRMP server bootstrap for a SLeeLa service.
 *
 * Usage:
 *   SleelaRmiServer <service-name> <sleela-executable> <working-directory> [registry-port]
 */
public final class SleelaRmiServer {
    private SleelaRmiServer() {
    }

    public static void main(String[] args) throws Exception {
        if (args.length < 3 || args.length > 4) {
            System.err.println("usage: SleelaRmiServer <service-name> <sleela-executable> <working-directory> [registry-port]");
            System.exit(2);
        }

        String name = args[0];
        Path executable = Path.of(args[1]);
        Path workingDirectory = Path.of(args[2]);
        int port = args.length == 4 ? Integer.parseInt(args[3]) : 1099;

        Registry registry = LocateRegistry.createRegistry(port);
        ProcessSleelaRuntime runtime = new ProcessSleelaRuntime(executable, workingDirectory);
        SleelaRmiService service = new SleelaRmiService(name, runtime);
        registry.rebind(name, service);

        System.out.println("SLeeLa RMI service registered: " + name + " on port " + port);
        System.out.println(service.health());
    }
}
