package implementations.one.nordshrift;

import implementations.one.bodi.BodiAnnotation;
import implementations.one.cord.Cord;
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

    @BodiAnnotation(requirement = "nordshrift.deferred.NordshriftSystemSequence")
    public NordshriftSystem()
    {
        Cord
                .reference
                .cord("{bodi}", "{nordshrift.NordshriftStarter}", "{start}")
                .cord("{bodi}", "{nordshrift.NordshriftLoader}", "{load}")
                .pack("{bodi}", "{nordshrift.NordshriftContext}", "{context}")
                .run();
    }
}