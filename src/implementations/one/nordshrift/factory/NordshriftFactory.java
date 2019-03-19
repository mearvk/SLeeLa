package implementations.one.nordshrift.factory;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.NordshriftAnnotation;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

public class NordshriftFactory extends ProtectedNordshriftFactory
{
    public NordshriftFactory()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());

        try
        {
            Class.forName("implementations.one.nordshrift.Nordshrift").newInstance();
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    @NordshriftAnnotation
    public void run()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }
}

