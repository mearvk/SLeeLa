package implementations._001_.bodi;

import java.io.File;
import java.io.FileInputStream;
import java.util.Properties;

/** Runtime configuration for Bodi's RMI and XML TCP endpoints. */
public final class BodiNetworkConfig
{
    public static final int DEFAULT_RMI_PORT = 8888;
    public static final int DEFAULT_BODI_PORT = 8890;
    public static final String DEFAULT_BIND_ADDRESS = "0.0.0.0";

    public final String bindAddress;
    public final int rmiPort;
    public final int bodiPort;
    public final boolean enabled;

    private BodiNetworkConfig(String bindAddress, int rmiPort, int bodiPort, boolean enabled)
    {
        this.bindAddress = bindAddress;
        this.rmiPort = rmiPort;
        this.bodiPort = bodiPort;
        this.enabled = enabled;
    }

    public static BodiNetworkConfig defaults()
    {
        return new BodiNetworkConfig(DEFAULT_BIND_ADDRESS, DEFAULT_RMI_PORT,
            DEFAULT_BODI_PORT, false);
    }

    public static BodiNetworkConfig load(File file) throws Exception
    {
        BodiNetworkConfig defaults = defaults();
        Properties properties = new Properties();
        if (file != null && file.isFile())
        {
            FileInputStream input = new FileInputStream(file);
            try { properties.load(input); } finally { input.close(); }
        }
        return new BodiNetworkConfig(
            properties.getProperty("bind.address", defaults.bindAddress),
            integer(properties, "rmi.port", defaults.rmiPort),
            integer(properties, "bodi.port", defaults.bodiPort),
            Boolean.parseBoolean(properties.getProperty("bodi.enabled", Boolean.toString(defaults.enabled)))
        );
    }

    private static int integer(Properties properties, String key, int fallback)
    {
        try { return Integer.parseInt(properties.getProperty(key, Integer.toString(fallback))); }
        catch (NumberFormatException exception) { return fallback; }
    }
}
