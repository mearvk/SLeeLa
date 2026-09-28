package implementations._001_.nordshrift.factory;

import implementations._001_.bodi.Bodi;
import implementations._001_.nordshrift.NordshriftAnnotation;
import implementations._001_.nordshrift.events.RegisterableNordshriftEvent;

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

