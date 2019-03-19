package implementations._001.nordshrift.descriptors;

import implementations._001.bodi.Bodi;

public class NordshriftConfigurationDescriptor
{
    public NordshriftConfigurationList list001;

    public NordshriftConfigurationDescriptor()
    {
        this.list001 = (NordshriftConfigurationList) Bodi.reference.pull("//descriptors/NordshriftConfigurationDescriptor");
    }
}
