package implementations._001.bodi;

import java.rmi.Remote;
import java.rmi.registry.Registry;
import java.util.HashMap;

public class BodiExtender
{
    public static Registry registry001;

    public static Registry registry002;

    public static HashMap<String, String> map001 = new HashMap<String, String>();

    public static HashMap<String, String> map002 = new HashMap<String, String>();

    public static HashMap<String, String> map003 = new HashMap<String, String>();

    //

    public BodiExtender()
    {
        this.map001.put("implementations._001.nordshrift.NordshriftStartup.<init>", "");

        this.map001.put("implementations._001.nordshrift.drivers.NordshriftStartup.<init>", "");
    }

    public Remote pull(String bodiref) throws Exception
    {
        return registry001.lookup(bodiref);
    }

    public void push(String bodiref, Remote remote) throws Exception
    {
        for(StackTraceElement element : Thread.currentThread().getStackTrace())
        {
            System.out.println(element);
        }

        registry001.bind(bodiref, remote);


    }
}
