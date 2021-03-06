package implementations._001_.nordshrift.drivers;

import implementations._001_.bodi.Bodi;
import implementations._001_.nordshrift.events.RegisterableNordshriftEvent;

import java.rmi.Remote;

public class NordshriftDriver extends NordshriftDriverExtent implements Remote
{
    public NordshriftDocument document001 = new NordshriftDocument("nordshrift.driver.ns");

    public NordshriftDocument document002 = new NordshriftDocument("nordshrift.ns");

    //

    public NordshriftDriver()
    {
        Bodi.reference.push("::", this, RegisterableNordshriftEvent.class);
    }

    public void init()
    {

    }
}
