package implementations._001.nordshrift.factory;

import implementations._001.bodi.Bodi;
import implementations._001.nordshrift.NordshriftAnnotation;
import implementations._001.nordshrift.events.RegisterableNordshriftEvent;

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

