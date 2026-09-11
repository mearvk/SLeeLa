package implementations._001_.bodi;

import java.io.*;
import java.net.Socket;
import java.util.function.Consumer;

/** Local Java-side listener for Gardulus.II JSON-lines telemetry. */
public final class GardulusII {
    private GardulusII() { }
    public static void listen(File logFile, Consumer<String> listener) throws IOException {
        if (logFile == null) throw new IllegalArgumentException("logFile is required");
        try (BufferedReader r = new BufferedReader(new FileReader(logFile))) { String line; while ((line=r.readLine())!=null) if(listener!=null) listener.accept(line); }
    }
    public static void listen(String host, int port, Consumer<String> listener) throws IOException {
        try (Socket s=new Socket(host,port); BufferedReader r=new BufferedReader(new InputStreamReader(s.getInputStream(),"UTF-8"))) { String line; while((line=r.readLine())!=null) if(listener!=null) listener.accept(line); }
    }
}
