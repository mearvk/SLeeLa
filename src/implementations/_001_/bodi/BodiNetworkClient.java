package implementations._001_.bodi;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

/** Small synchronous client for SLeeLa-aware Bodi endpoints. */
public final class BodiNetworkClient
{
    private BodiNetworkClient() { }

    public static String send(String host, int port, String xml) throws Exception
    {
        try (Socket socket = new Socket(host, port);
             BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8));
             BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8)))
        {
            writer.write(xml.replace("\r", "").replace("\n", ""));
            writer.newLine();
            writer.flush();
            return reader.readLine();
        }
    }
}
