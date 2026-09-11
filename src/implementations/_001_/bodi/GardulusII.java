package implementations._001_.bodi;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.function.Consumer;

/** Local Java-side listener for Gardulus.II JSON-lines telemetry. */
public final class GardulusII
{
    private GardulusII() { }

    public static void listen(File logFile, Consumer<String> listener) throws IOException
    {
        if (logFile == null) throw new IllegalArgumentException("logFile is required");
        try (BufferedReader reader = new BufferedReader(new FileReader(logFile)))
        {
            String line;
            while ((line = reader.readLine()) != null)
                if (listener != null) listener.accept(line);
        }
    }

    public static void listen(String host, int port, Consumer<String> listener) throws IOException
    {
        try (Socket socket = new Socket(host, port);
             BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream(), "UTF-8")))
        {
            String line;
            while ((line = reader.readLine()) != null)
                if (listener != null) listener.accept(line);
        }
    }
}
