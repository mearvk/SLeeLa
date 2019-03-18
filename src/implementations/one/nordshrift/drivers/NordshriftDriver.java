package implementations.one.nordshrift.drivers;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;
import implementations.one.nordshrift.factory.NordshriftFactory;

public class NordshriftDriver
{
    public NordshriftDocument document001 = new NordshriftDocument("implementations.one.documents/nordshrift.ns");

    public NordshriftDriver()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }

    public void factory()
    {
        NordshriftFactory factory = new NordshriftFactory();
    }
}
