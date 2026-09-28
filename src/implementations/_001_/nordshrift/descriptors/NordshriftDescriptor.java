package implementations._001_.nordshrift.descriptors;

import implementations._001_.bodi.Bodi;
import implementations._001_.nordshrift.events.RegisterableNordshriftEvent;

public class NordshriftDescriptor extends GenericDescriptor
{
    static
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }

    public NordshriftDescriptor()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }
}
