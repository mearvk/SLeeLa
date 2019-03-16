package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.extenders.NordshriftExtender;

public class NordshriftSystem
{
    @NordshriftAnnotation(id=1)
    public NordshriftExtender extender001 = new NordshriftExtender(this, "{}"); //

    @NordshriftAnnotation(id=2)
    public NordshriftExtender extender002 = new NordshriftExtender(this, "{}"); //

    @NordshriftAnnotation(id=3)
    public NordshriftExtender extender003 = new NordshriftExtender(this, "{}"); //

    @NordshriftAnnotation(id=4)
    public NordshriftExtender extender004 = new NordshriftExtender(this, "{}"); //

    //

    @NordshriftAnnotation(id=1)
    public NordshriftMonitor monitor001 = new NordshriftMonitor(this,"{}");     //

    @NordshriftAnnotation(id=2)
    public NordshriftMonitor monitor002 = new NordshriftMonitor(this, "{}");    //

    @NordshriftAnnotation(id=3)
    public NordshriftMonitor monitor003 = new NordshriftMonitor(this, "{}");    //

    //

    @NordshriftAnnotation(ref="::")
    public NordshriftSystem()
    {
        Bodi.run("Nordshrift", "::", "fill");                   //
    }

    @NordshriftAnnotation
    public void load(String file)
    {

    }
}