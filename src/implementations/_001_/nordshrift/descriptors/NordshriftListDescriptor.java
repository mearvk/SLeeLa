package implementations._001_.nordshrift.descriptors;

import implementations._001_.bodi.Bodi;

public class NordshriftListDescriptor
{
    public NordshriftDescriptorList list001 = new NordshriftDescriptorList();

    public NordshriftListDescriptor()
    {
        Bodi.reference.push("//descriptors/NordshriftListDescriptor", this.list001);
    }
}
