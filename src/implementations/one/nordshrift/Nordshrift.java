package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.descriptors.NordshriftDescriptor;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

@NordshriftAnnotation
public class Nordshrift extends NordshriftSystem
{
    public static final String quickname = "$";

    public static final String name = "nordshrift";

    //

    @NordshriftAnnotation
    public NordshriftDescriptor descriptor001;

    //

    @NordshriftAnnotation
    public NordshriftSystem nordshriftsystem001;

    //

    @NordshriftAnnotation
    public Nordshrift nordshrift001;

    @NordshriftAnnotation
    public Nordshrift nordshrift002;

    @NordshriftAnnotation
    public Nordshrift nordshrift003;

    @NordshriftAnnotation
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