package implementations._001.models;

import implementations._001.bodi.Bodi;
import implementations._001.modules.Modules;
import implementations._001.modules.Processables;
import implementations._001.modules.Submodules;
import implementations._001.nordshrift.drivers.NordshriftDriverExtent;
import implementations._001.nordshrift.events.RegisterableNordshriftEvent;

import java.rmi.Remote;

public class NordshriftModel implements Remote
{
    public NordshriftDriverExtent driver001;

    public Modules modules001;

    public Submodules submodules001;

    public Processables processables001;

    //

    public NordshriftModel(NordshriftDriverExtent driver)
    {
        Bodi.reference.push("::", this, RegisterableNordshriftEvent.class);
    }
}
