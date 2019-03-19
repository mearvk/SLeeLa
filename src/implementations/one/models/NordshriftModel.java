package implementations.one.models;

import implementations.one.modules.Modules;
import implementations.one.modules.Submodules;
import implementations.one.nordshrift.drivers.NordshriftDriver;
import implementations.one.nordshrift.drivers.NordshriftDriverExtent;

public class NordshriftModel
{
    public NordshriftDriverExtent driver001;

    public Modules modules001;

    public Submodules submodules001;

    //

    public NordshriftModel(NordshriftDriverExtent driver)
    {
        this.driver001 = driver;
    }
}
