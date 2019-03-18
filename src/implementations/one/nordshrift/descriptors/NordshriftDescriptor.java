package implementations.one.nordshrift.descriptors;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

public class NordshriftDescriptor
{
    //public NordshriftListDescriptor descriptor001;

    public NordshriftInstanceDescriptor descriptor002;

    //public NordshriftConfigurationDescriptor descriptor003;

    //

    static
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }

    public NordshriftDescriptor()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }
}
