package implementations.one.bodi;

import implementations.one.nordshrift.NordshriftSystem;
import implementations.one.nordshrift.threading.NordshriftThreading;

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
        StackTraceElement[] stacktrace001 = Thread.currentThread().getStackTrace();

        //

        NordshriftThreading threads = NordshriftSystem.getThreadController();

        //

        for(StackTraceElement element : stacktrace001) //
        {
            if(element.getClassName().equals("NordshriftStartup"))
            {
                this.SUGGESTED_CAUSE += 0b0001;
            }
        }

        for(StackTraceElement element : stacktrace001) //
        {
            if(element.getClassName().equals("NordshriftDriver"))
            {
                this.SUGGESTED_CAUSE += 0b0010;
            }
        }
    }
}

