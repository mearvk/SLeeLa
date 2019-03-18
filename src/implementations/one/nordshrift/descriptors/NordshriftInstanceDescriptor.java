package implementations.one.nordshrift.descriptors;

import implementations.one.bodi.Bodi;

public class NordshriftInstanceDescriptor
{
    public NordshriftInstancingList list001;

    public NordshriftInstanceDescriptor()
    {
        this.list001 = (NordshriftInstancingList)Bodi.reference.pull("//descriptors/NordshriftInstanceDescriptor");
    }
}
