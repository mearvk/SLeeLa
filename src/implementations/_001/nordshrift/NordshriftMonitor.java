package implementations._001.nordshrift;

public class NordshriftMonitor
{
    public Thread thread;

    public String type;

    public NordshriftSystem system;

    public NordshriftMonitor(NordshriftSystem system, String type)
    {
        this.system = system;

        this.type = type;
    }
}
