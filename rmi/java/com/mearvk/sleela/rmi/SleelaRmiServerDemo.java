package com.mearvk.sleela.rmi;

import com.mearvk.sleela.gui.SleelaRuntime;

/** In-process demonstration of the complete server lifecycle. */
public final class SleelaRmiServerDemo {
    private SleelaRmiServerDemo() {
    }

    public static void main(String[] args) throws Exception {
        SleelaRuntime runtime = new SleelaRuntime() {
            @Override
            public Object call(String operation, Object... arguments) {
                if ("main".equals(operation)) {
                    return "SLeeLa server executed main";
                }
                if ("echo".equals(operation)) {
                    return arguments.length == 0 ? "" : String.valueOf(arguments[0]);
                }
                return "unknown operation: " + operation;
            }

            @Override
            public void close() {
            }
        };

        try (SleelaRmiServerHandle server = SleelaRmiServerHandle.start("SLeeLa-Demo", 1099, runtime)) {
            System.out.println(server.health());
            try (SleelaRmiClient client = new SleelaRmiClient(
                    SleelaRmiEndpoint.local("SLeeLa-Demo", 1099))) {
                System.out.println(client.health());
                System.out.println(client.invoke("echo", "RMI round trip"));
            }
        } finally {
            runtime.close();
        }
    }
}
