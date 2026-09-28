package implementations._001_.models;

import implementations._001_.bodi.Bodi;
import implementations._001_.modules.Modules;
import implementations._001_.modules.Processables;
import implementations._001_.modules.Submodules;
import implementations._001_.nordshrift.drivers.NordshriftDriverExtent;
import implementations._001_.nordshrift.events.RegisterableNordshriftEvent;

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
