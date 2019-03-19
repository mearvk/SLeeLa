package implementations.one.models;

import implementations.one.bodi.Bodi;
import implementations.one.modules.Modules;
import implementations.one.modules.Processables;
import implementations.one.modules.Submodules;
import implementations.one.nordshrift.drivers.NordshriftDriverExtent;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

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
