package implementations.one.system;

public class System
{
    private static boolean include_static = false;

    public static void list(Class klass)
    {
        if(include_static)
            java.lang.System.out.println("[static] "+klass);
    }

    public static void list(Object object)
    {
        java.lang.System.out.println("[instance] "+object);
    }
}
