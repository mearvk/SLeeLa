package implementations._001.nordshrift.drivers;

import implementations._001.bodi.Bodi;
import implementations._001.nordshrift.events.RegisterableNordshriftEvent;

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
