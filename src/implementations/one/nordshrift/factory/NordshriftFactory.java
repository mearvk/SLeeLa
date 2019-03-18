package implementations.one.nordshrift.factory;

public class NordshriftFactory extends ProtectedNordshriftFactory
{
    private final Integer PROTECTED_DIRECT = 1;

    private final Integer PROTECTED_NONDIRECT = 2;

    private final Integer NONPROTECTED_DIRECT = 4;

    private final Integer NONPROTECTED_NONDIRECT = 8;

    //

    private Integer SUGGESTED_CAUSE = 0b0000;

    //

    public NordshriftFactory()
    {
        java.lang.System.out.println("[factory] checking for threading config ... ");

        java.lang.System.out.println("[factory] checking for threading config(s) ... ");

        java.lang.System.out.println("[factory] checking for dynamic loader(s) ... ");

        java.lang.System.out.println("[factory] checking for java agent(s) ... ");

        //

        new Thread()
        {
            public void run()
            {
                try
                {
                    Class.forName("implementations.one.nordshrift.Nordshrift").newInstance();
                }
                catch (Exception exception)
                {
                    java.lang.System.out.println(exception);
                }
            }

        }.start();
    }
}

