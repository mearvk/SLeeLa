package implementations._001_.bodi;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/** TCP bridge for the agreed Bodi XML request/response schema. */
public final class BodiNetworkServer implements AutoCloseable
{
    private final BodiExtender extender;
    private final ExecutorService workers;
    private volatile boolean running;
    private ServerSocket server;

    public BodiNetworkServer(BodiExtender extender)
    {
        this.extender = extender;
        this.workers = Executors.newCachedThreadPool();
    }

    public synchronized void start(int port) throws Exception
    {
        if (running)
            return;
        server = new ServerSocket(port);
        running = true;
        workers.submit(new Runnable()
        {
            public void run()
            {
                acceptLoop();
            }
        });
    }

    public int port()
    {
        return server == null ? -1 : server.getLocalPort();
    }

    private void acceptLoop()
    {
        while (running)
        {
            try
            {
                final Socket socket = server.accept();
                workers.submit(new Runnable()
                {
                    public void run()
                    {
                        serve(socket);
                    }
                });
            }
            catch (Exception exception)
            {
                if (running)
                    exception.printStackTrace();
            }
        }
    }

    private void serve(Socket socket)
    {
        try (Socket current = socket;
             BufferedReader reader = new BufferedReader(new InputStreamReader(current.getInputStream(), StandardCharsets.UTF_8));
             BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(current.getOutputStream(), StandardCharsets.UTF_8)))
        {
            String xml = reader.readLine();
            if (xml == null)
                return;

            BodiChange change = BodiXmlDocument.parse(xml);
            Object result = extender.invoke(change, "network", "bodi-network");
            writer.write(BodiXmlDocument.response("ok", result == null ? "null" : String.valueOf(result)));
            writer.newLine();
            writer.flush();
        }
        catch (Exception exception)
        {
            try
            {
                BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8));
                writer.write(BodiXmlDocument.response("error", exception.toString()));
                writer.newLine();
                writer.flush();
            }
            catch (Exception ignored)
            {
                // Connection may already be closed.
            }
        }
    }

    public synchronized void close()
    {
        running = false;
        if (server != null)
        {
            try { server.close(); } catch (Exception ignored) { }
            server = null;
        }
        workers.shutdownNow();
    }
}
