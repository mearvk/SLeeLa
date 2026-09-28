package implementations._001_.bodi;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
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
        start(BodiNetworkConfig.DEFAULT_BIND_ADDRESS, port);
    }

    public synchronized void start(String bindAddress, int port) throws Exception
    {
        if (running)
            return;
        InetAddress address = InetAddress.getByName(bindAddress);
        server = new ServerSocket(port, 50, address);
        running = true;
        workers.submit(new Runnable()
        {
            public void run() { acceptLoop(); }
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
                    public void run() { serve(socket); }
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

            try
            {
                BodiChange change = BodiXmlDocument.parse(xml);
                Object result = extender.invoke(change,
                    change.starter.length() == 0 ? "network" : change.starter,
                    change.man.length() == 0 ? "bodi-network" : change.man);
                writer.write(BodiXmlDocument.response("ok", result == null ? "null" : String.valueOf(result)));
            }
            catch (Exception exception)
            {
                writer.write(BodiXmlDocument.response("error", exception.toString()));
            }
            writer.newLine();
            writer.flush();
        }
        catch (Exception ignored)
        {
            // Connection failure is isolated to this worker.
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
