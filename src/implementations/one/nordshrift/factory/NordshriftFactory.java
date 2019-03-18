package implementations.one.nordshrift.factory;

import implementations.one.bodi.Bodi;

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
        Bodi.push("", new RegisterableNordshriftEvent());

        //

        java.lang.System.out.println("  [ns::security]   Nordshrift Threading Model™ safeguarded");

        java.lang.System.out.println("  [ns::security]   Nordshrift Classpath Model™ safeguarded");

        java.lang.System.out.println("  [ns::security]   Nordshrift Classloader Model™ safeguarded");

        java.lang.System.out.println("  [ns::security]   Nordshrift JavaAgent Model™ safeguarded");

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

