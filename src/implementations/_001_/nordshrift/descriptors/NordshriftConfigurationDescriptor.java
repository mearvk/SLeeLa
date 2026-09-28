package implementations._001_.nordshrift.descriptors;

import implementations._001_.bodi.Bodi;

public class NordshriftConfigurationDescriptor
{
    public NordshriftConfigurationList list001;

    public NordshriftConfigurationDescriptor()
    {
        this.list001 = (NordshriftConfigurationList) Bodi.reference.pull("//descriptors/NordshriftConfigurationDescriptor");
    }
}
