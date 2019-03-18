package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;

public class NordshriftInstanceDescriptor
{
    public GuardedList list001;

    public NordshriftInstanceDescriptor()
    {
        this.list001 = (GuardedList)Bodi.pull("//descriptors/NordshriftInstanceDescriptor");
    }
}
