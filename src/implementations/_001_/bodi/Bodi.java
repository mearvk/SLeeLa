package implementations._001_.bodi;

import implementations._001_.nordshrift.events.RegisterableNordshriftEvent;
import java.rmi.Remote;
import java.rmi.registry.LocateRegistry;

public class Bodi extends BodiExtent implements Remote
{
    public static Bodi reference = new Bodi();

    public Bodi bodi = reference;

    //

    static
    {
        //System.out.println("  [ns::bodi]       Bodi Registry™ initialized");

        try
        {
            BodiExtender.registry001 = LocateRegistry.createRegistry(8888);

            BodiExtender.registry001.bind("@bodi", Bodi.reference);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    public void run(Encapsulator encapsulator)
    {

    }

    public void run(String protocol, String bodiref, String methodname)
    {

    }

    public void run(String protocol, String bodiref, String methodname, StackTraceElement[] stack)
    {

    }

    //

    public Remote pull(String bodiref)
    {
        try
        {
            return this.extender001.pull(bodiref);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }

        return null;
    }

    public void push(String bodiref, Remote remote)
    {
        try
        {
            this.extender001.push(bodiref, remote);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    public void push(String bodiref, Class klass)
    {
        try
        {
            //this.extender001.push(bodiref, remote);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    public void push(String bodiref, Remote remote, RegisterableNordshriftEvent event)
    {
        try
        {
            this.extender001.push(bodiref, remote);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    public void push(String bodiref, Remote remote, Class<RegisterableNordshriftEvent> klass)
    {
        try
        {
            this.extender001.push(bodiref, remote);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    class Encapsulator
    {
        public StringBuffer buffer = new StringBuffer();

        public Encapsulator(StackTraceElement[] stacktrace)
        {
            for(StackTraceElement element : stacktrace)
            {
                this.buffer.append(element);
            }
        }

        public boolean contains(String string)
        {
            return (this.buffer.toString().contains(string));
        }
    }
}

class BodiExtent
{
    public BodiExtender extender001 = new BodiExtender();
}
