package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.descriptors.NordshriftDescriptor;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

public class Nordshrift extends NordshriftSystem
{
    public static final String quickname = "$";

    public static final String name = "nordshrift";

    //

    public NordshriftDescriptor descriptor001;

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