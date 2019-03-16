package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.extenders.NordshriftExtender;

public class NordshriftSystem
{
    public NordshriftExtender extender001 = new NordshriftExtender(this, "functional");

    public NordshriftExtender extender002 = new NordshriftExtender(this, "factorial");

    public NordshriftExtender extender003 = new NordshriftExtender(this, "loader");

    public NordshriftExtender extender004 = new NordshriftExtender(this, "starter");

    //

    public NordshriftMonitor monitor001 = new NordshriftMonitor("startup");

    public NordshriftMonitor monitor002 = new NordshriftMonitor("runtime");

    public NordshriftMonitor monitor003 = new NordshriftMonitor("shutdown");

    //

    public NordshriftSystem()
    {
        Bodi.run("{bodi}", "{nordshriftloader}", "{load}");
    }
}