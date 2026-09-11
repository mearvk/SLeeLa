package implementations._001_.bodi;

import implementations._001_.nordshrift.events.RegisterableNordshriftEvent;
import java.rmi.Remote;
import java.rmi.registry.LocateRegistry;

/**
 * Bodi is the witness layer for change to an addressed object.
 *
 * Preferred form:
 * Bodi.system("xxx").propagative("001").install("yyy");
 *
 * Java RMI remains the transport/registry compatibility layer underneath it.
 */
public class Bodi extends BodiExtent implements Remote
{
    public static Bodi reference = new Bodi();
    public Bodi bodi = reference;

    static
    {
        try
        {
            BodiExtender.registry001 = LocateRegistry.createRegistry(BodiNetworkConfig.DEFAULT_RMI_PORT);
            BodiExtender.registry001.rebind("@bodi", Bodi.reference);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    public BodiObjectReference system(String name)
    {
        return this.extender001.system(name);
    }

    public BodiNetworkServer startNetwork(BodiNetworkConfig config) throws Exception
    {
        BodiNetworkServer server = new BodiNetworkServer(this.extender001);
        if (config != null && config.enabled)
            server.start(config.bodiPort);
        return server;
    }

    public void run(Encapsulator encapsulator)
    {
        // Witness execution is represented by the fluent API and invoke().
    }

    public void run(String protocol, String bodiref, String methodname)
    {
        try
        {
            this.extender001.invoke(new BodiChange(bodiref, methodname, "", ""),
                protocol, "run");
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    public void run(String protocol, String bodiref, String methodname, StackTraceElement[] stack)
    {
        run(protocol, bodiref, methodname);
    }

    public Remote pull(String bodiref)
    {
        try { return this.extender001.pull(bodiref); }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
            return null;
        }
    }

    public void push(String bodiref, Remote remote)
    {
        try { this.extender001.push(bodiref, remote); }
        catch (Exception exception) { java.lang.System.out.println(exception); }
    }

    public void push(String bodiref, Class klass)
    {
        try
        {
            Remote remote = (Remote) klass.getDeclaredConstructor().newInstance();
            this.extender001.push(bodiref, remote);
        }
        catch (Exception exception) { java.lang.System.out.println(exception); }
    }

    public void push(String bodiref, Remote remote, RegisterableNordshriftEvent event)
    {
        push(bodiref, remote);
    }

    public void push(String bodiref, Remote remote, Class<RegisterableNordshriftEvent> klass)
    {
        push(bodiref, remote);
    }

    class Encapsulator
    {
        public StringBuffer buffer = new StringBuffer();

        public Encapsulator(StackTraceElement[] stacktrace)
        {
            for (StackTraceElement element : stacktrace)
                this.buffer.append(element);
        }

        public boolean contains(String string)
        {
            return this.buffer.toString().contains(string);
        }
    }
}

class BodiExtent
{
    public BodiExtender extender001 = new BodiExtender();
}
