package implementations.one.nordshrift.descriptors;

import implementations.one.bodi.Bodi;

public class NordshriftListDescriptor
{
    public NordshriftDescriptorList list001 = new NordshriftDescriptorList();

    public NordshriftListDescriptor()
    {
        Bodi.push("//descriptors/NordshriftListDescriptor", this.list001);
    }
}
