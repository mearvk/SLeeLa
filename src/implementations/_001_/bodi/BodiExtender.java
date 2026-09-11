package implementations._001_.bodi;

import implementations._001_.nordshrift.drivers.NordshriftDriver;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.rmi.Remote;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/** Backend for Bodi's system/object address space. */
public class BodiExtender
{
    public static Registry registry001;
    public static Registry registry002;
    public static final HashMap<String, ReflectionItem> map001 = new HashMap<String, ReflectionItem>();
    public static final HashMap<String, ReflectionItem> map002 = new HashMap<String, ReflectionItem>();
    public static final HashMap<String, ReflectionItem[]> map003 = new HashMap<String, ReflectionItem[]>();

    private final Map<String, List<BodiWitness>> witnesses =
        Collections.synchronizedMap(new HashMap<String, List<BodiWitness>>());

    public BodiExtender()
    {
        try
        {
            map002.put("implementations._001_.nordshrift.NordshriftStartup.<init>",
                new ReflectionItem(NordshriftDriver.class.getConstructor()));
            map001.put("implementations._001_.nordshrift.drivers.NordshriftDriver.init",
                new ReflectionItem(NordshriftDriver.class.getMethod("init")));
        }
        catch (Exception exception) { }
    }

    public Remote pull(String bodiref) throws Exception
    {
        return resolveSystem(bodiref);
    }

    public void push(String bodiref, Remote remote) throws Exception
    {
        ensureRegistry();
        registry001.rebind(bodiref, remote);
    }

    public Remote resolveSystem(String system) throws Exception
    {
        ensureRegistry();
        return registry001.lookup(system);
    }

    public String describe(String system) throws Exception
    {
        return BodiDescriptor.describe(system, resolveSystem(system));
    }

    public BodiObjectReference system(String name)
    {
        return new BodiObjectReference(this, name);
    }

    public BodiWitness witness(String system, String reference, String change,
                               String method, String sequence, String starter, String man)
    {
        BodiWitness witness = new BodiWitness(system, reference, change, method,
            sequence, starter, man, path(system, sequence, method));
        synchronized (witnesses)
        {
            List<BodiWitness> list = witnesses.get(system);
            if (list == null)
            {
                list = new ArrayList<BodiWitness>();
                witnesses.put(system, list);
            }
            list.add(witness);
        }
        return witness;
    }

    public List<BodiWitness> witnesses(String system)
    {
        synchronized (witnesses)
        {
            List<BodiWitness> list = witnesses.get(system);
            return list == null ? Collections.<BodiWitness>emptyList()
                : Collections.unmodifiableList(new ArrayList<BodiWitness>(list));
        }
    }

    /** Invoke an operation on a locally registered or RMI-resolved object. */
    public Object invoke(BodiChange change, String starter, String man) throws Exception
    {
        if (change.reference.length() == 0)
            throw new IllegalArgumentException("Bodi reference is required");
        if (change.method.length() == 0)
            throw new IllegalArgumentException("Bodi method is required");

        Remote target = resolveSystem(change.reference);
        BodiWitness witness = witness(change.reference, change.reference,
            "object-change", change.method, change.sequence, starter, man);
        Object result = invokeMethod(target, change.method, change.datum);
        return result == null ? witness.toString() : result;
    }

    private Object invokeMethod(Object target, String methodName, String datum) throws Exception
    {
        Method stringMethod = null;
        Method noArgMethod = null;
        for (Method method : target.getClass().getMethods())
        {
            if (!method.getName().equals(methodName))
                continue;
            Class<?>[] parameters = method.getParameterTypes();
            if (parameters.length == 1 && parameters[0].isAssignableFrom(String.class))
            {
                stringMethod = method;
                break;
            }
            if (parameters.length == 0)
                noArgMethod = method;
        }
        if (stringMethod != null)
            return stringMethod.invoke(target, datum);
        if (noArgMethod != null)
            return noArgMethod.invoke(target);
        throw new NoSuchMethodException(methodName);
    }

    private List<String> path(String system, String sequence, String method)
    {
        List<String> path = new ArrayList<String>();
        path.add(system == null ? "" : system);
        if (sequence != null && sequence.length() > 0)
            path.add(sequence);
        if (method != null && method.length() > 0)
            path.add(method);
        return path;
    }

    private static synchronized void ensureRegistry() throws Exception
    {
        if (registry001 == null)
            registry001 = LocateRegistry.createRegistry(BodiNetworkConfig.DEFAULT_RMI_PORT);
    }

    class ReflectionItem
    {
        public final Method method;
        public final Constructor constructor;
        public final Field field;

        public ReflectionItem(Method method)
        {
            this.method = method;
            this.constructor = null;
            this.field = null;
        }

        public ReflectionItem(Constructor constructor)
        {
            this.method = null;
            this.constructor = constructor;
            this.field = null;
        }

        public ReflectionItem(Field field)
        {
            this.method = null;
            this.constructor = null;
            this.field = field;
        }
    }
}
