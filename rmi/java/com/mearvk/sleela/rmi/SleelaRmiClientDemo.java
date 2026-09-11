package com.mearvk.sleela.rmi;

/** Minimal client demonstration. */
public final class SleelaRmiClientDemo {
    private SleelaRmiClientDemo() {
    }

    public static void main(String[] args) throws Exception {
        String host = args.length > 0 ? args[0] : "127.0.0.1";
        int port = args.length > 1 ? Integer.parseInt(args[1]) : 1099;
        String service = args.length > 2 ? args[2] : "SLeeLa";

        try (SleelaRmiClient client = new SleelaRmiClient(host, port, service)) {
            System.out.println(client.health());
            System.out.println("service = " + client.serviceName());
            System.out.println("result = " + client.invoke("main", ""));
        }
    }
}
