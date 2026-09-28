package implementations._001_.system;

public class System
{
    private static boolean include_static = false;

    public static void list(Class klass)
    {
        if(include_static)
            java.lang.System.out.println("  [ns::static]   "+klass);
    }

    public static void list(Object object)
    {
        java.lang.System.out.println("  [ns::instance]   "+object);
    }
}
