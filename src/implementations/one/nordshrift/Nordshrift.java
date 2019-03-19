package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.descriptors.NordshriftDescriptor;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

@NordshriftAnnotation
public class Nordshrift
{
    public static final String quickname = "$";

    public static final String name = "nordshrift";

    //

    public NordshriftDescriptor descriptor001;      //preinit

    public NordshriftDescriptor descriptor002;      //init

    public NordshriftDescriptor descriptor003;      //start

    public NordshriftDescriptor descriptor004;      //monitor

    //

    public NordshriftSystem nordshriftsystem001;

    public NordshriftSystem nordshriftsystem002;

    public NordshriftSystem nordshriftsystem003;

    public NordshriftSystem nordshriftsystem004;

    //

    public Nordshrift nordshrift001;

    public Nordshrift nordshrift002;

    public Nordshrift nordshrift003;

    public Nordshrift nordshrift004;

    //

    static
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }

    //

    public Nordshrift()
    {
        Bodi.reference.push("::", new RegisterableNordshriftEvent());
    }
}