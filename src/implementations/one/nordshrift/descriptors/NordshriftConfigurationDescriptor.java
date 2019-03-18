package implementations.one.nordshrift.descriptors;

import implementations.one.bodi.Bodi;

public class NordshriftConfigurationDescriptor
{
    public NordshriftConfigurationList list001;

    public NordshriftConfigurationDescriptor()
    {
        this.list001 = (NordshriftConfigurationList) Bodi.pull("//descriptors/NordshriftConfigurationDescriptor");
    }
}
