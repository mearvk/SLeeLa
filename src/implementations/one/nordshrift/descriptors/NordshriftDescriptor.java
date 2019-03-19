package implementations.one.nordshrift.descriptors;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

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
