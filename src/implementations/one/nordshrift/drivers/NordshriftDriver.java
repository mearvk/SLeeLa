package implementations.one.nordshrift.drivers;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

import java.rmi.Remote;

public class NordshriftDriver implements Remote
{
    public NordshriftDocument document001 = new NordshriftDocument("nordshrift.driver.ns");

    public NordshriftDocument document002 = new NordshriftDocument("nordshrift.ns");

    //

    public NordshriftDriver()
    {
        Bodi.reference.push("::", this, new RegisterableNordshriftEvent());
    }
}
