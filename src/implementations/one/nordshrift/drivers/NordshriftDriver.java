package implementations.one.nordshrift.drivers;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

public class NordshriftDriver
{
    public NordshriftDocument document001 = new NordshriftDocument("nordshrift.driver.ns");

    public NordshriftDocument document002 = new NordshriftDocument("nordshrift.ns");

    //

    public NordshriftDriver()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }
}
