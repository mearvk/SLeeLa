package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.extenders.NordshriftExtender;
import implementations.one.nordshrift.threading.NordshriftThreading;

@NordshriftAnnotation
public class NordshriftSystem
{
    @NordshriftAnnotation
    public NordshriftExtender extender001 = new NordshriftExtender(this, "{nordshrift:extender}");  //

    @NordshriftAnnotation
    public NordshriftExtender extender002 = new NordshriftExtender(this, "{nordshrift:extender}");  //

    @NordshriftAnnotation
    public NordshriftExtender extender003 = new NordshriftExtender(this, "{nordshrift::extender}"); //

    @NordshriftAnnotation
    public NordshriftExtender extender004 = new NordshriftExtender(this, "{nordshrift::extender}"); //

    //

    @NordshriftAnnotation
    public NordshriftMonitor monitor001 = new NordshriftMonitor(this, "{nordshrift::monitor}");     //

    @NordshriftAnnotation
    public NordshriftMonitor monitor002 = new NordshriftMonitor(this, "{nordshrift::monitor}");     //

    @NordshriftAnnotation
    public NordshriftMonitor monitor003 = new NordshriftMonitor(this, "{nordshrift::monitor}");     //

    //

    @NordshriftAnnotation
    public static NordshriftThreading threading = new NordshriftThreading();

    //

    @NordshriftAnnotation(ref="::")
    public NordshriftSystem()
    {
        Bodi.run("Nordshrift", "::", "fill");                                       //
    }

    //

    public static NordshriftThreading getThreadController()
    {
        if(threading==null) return null;

        return threading;
    }

    //

    @NordshriftAnnotation
    public void start(String file)
    {
        //this.extender001.start(file);
    }

    @NordshriftAnnotation
    public void load(String file)
    {
        //this.extender001.load(file);
    }

    @NordshriftAnnotation
    public void function(String file)
    {
        //this.extender003.functional(file);
    }

    @NordshriftAnnotation
    public void factorial(String file)
    {
        //this.extender004.factorial(file);
    }
}