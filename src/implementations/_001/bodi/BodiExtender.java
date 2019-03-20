package implementations._001.bodi;

import implementations._001.nordshrift.drivers.NordshriftDriver;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.rmi.Remote;
import java.rmi.registry.Registry;
import java.util.HashMap;

public class BodiExtender
{
    public static Registry registry001;

    public static Registry registry002;


    public static HashMap<String, ReflectionItem> map001 = new HashMap<String, ReflectionItem>();

    public static HashMap<String, ReflectionItem> map002 = new HashMap<String, ReflectionItem>();

    public static HashMap<String, ReflectionItem[]> map003 = new HashMap<String, ReflectionItem[]>();

    //

    public BodiExtender()
    {
        try
        {
            this.map002.put("implementations._001.nordshrift.NordshriftStartup.<init>", new ReflectionItem(NordshriftDriver.class.getConstructor()));

            this.map001.put("implementations._001.nordshrift.drivers.NordshriftDriver.<init>", new ReflectionItem(NordshriftDriver.class.getMethod("init")));
        }
        catch (Exception exception)
        {

        }
    }

    public Remote pull(String bodiref) throws Exception
    {
        return registry001.lookup(bodiref);
    }

    public void push(String bodiref, Remote remote) throws Exception
    {
        registry001.bind(bodiref, remote);
    }

    class ReflectionItem
    {
        public ReflectionItem(Method method)
        {

        }

        public ReflectionItem(Constructor constructor)
        {

        }

        public ReflectionItem(Field field)
        {

        }
    }
}
