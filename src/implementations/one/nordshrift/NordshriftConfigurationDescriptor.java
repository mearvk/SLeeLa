package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;

public class NordshriftConfigurationDescriptor
{
    public GuardedList list001;

    public NordshriftConfigurationDescriptor()
    {
        this.list001 = (GuardedList) Bodi.pull("//descriptors/NordshriftConfigurationDescriptor");
    }
}
