package implementations._001_.bodi;

import java.io.File;

/** Starts Bodi networking from a properties configuration file. */
public final class BodiNetworkBootstrap
{
    private BodiNetworkBootstrap() { }

    public static void main(String[] args) throws Exception
    {
        File configFile = new File(args.length == 0
            ? "bodi.properties"
            : args[0]);
        BodiNetworkConfig config = BodiNetworkConfig.load(configFile);
        Bodi bodi = Bodi.reference;
        if (bodi == null)
            throw new IllegalStateException("Bodi runtime failed to initialize");

        BodiNetworkServer server = bodi.startNetwork(config);
        if (!config.enabled)
        {
            System.out.println("Bodi network disabled by configuration.");
            return;
        }

        System.out.println("Bodi XML network listening on " + config.bindAddress + ":" + server.port());
        try
        {
            synchronized (BodiNetworkBootstrap.class)
            {
                BodiNetworkBootstrap.class.wait();
            }
        }
        finally
        {
            server.close();
        }
    }
}
