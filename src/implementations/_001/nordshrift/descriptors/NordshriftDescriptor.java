package implementations._001.nordshrift.descriptors;

import implementations._001.bodi.Bodi;
import implementations._001.nordshrift.events.RegisterableNordshriftEvent;

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
