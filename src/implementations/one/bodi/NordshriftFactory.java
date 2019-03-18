package implementations.one.bodi;

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
        //check if nordshrift has been configured to run with threading

        //scan classpath to find references to instantiation that are not protected

        //dump stacktrace to check if dynamic load has been used
    }
}

