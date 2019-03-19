package implementations.one.nordshrift.factory;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.NordshriftAnnotation;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

public class NordshriftFactory extends ProtectedNordshriftFactory implements Runnable
{
    public NordshriftFactory()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }

    @NordshriftAnnotation
    public void run()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }
}

