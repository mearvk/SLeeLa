package implementations._001.nordshrift;

import implementations._001.bodi.Bodi;
import implementations._001.nordshrift.descriptors.NordshriftDescriptor;
import implementations._001.nordshrift.events.RegisterableNordshriftEvent;

@NordshriftAnnotation
public class Nordshrift
{
    public static final String quickname = "$";

    public static final String name = "nordshrift";

    //

    public NordshriftDescriptor descriptor001;

    public NordshriftDescriptor descriptor002;

    public NordshriftDescriptor descriptor003;

    public NordshriftDescriptor descriptor004;

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