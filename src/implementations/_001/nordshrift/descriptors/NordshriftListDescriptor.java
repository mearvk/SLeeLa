package implementations._001.nordshrift.descriptors;

import implementations._001.bodi.Bodi;

public class NordshriftListDescriptor
{
    public NordshriftDescriptorList list001 = new NordshriftDescriptorList();

    public NordshriftListDescriptor()
    {
        Bodi.reference.push("//descriptors/NordshriftListDescriptor", this.list001);
    }
}
