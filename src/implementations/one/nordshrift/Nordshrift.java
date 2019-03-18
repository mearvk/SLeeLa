package implementations.one.nordshrift;

import implementations.one.nordshrift.descriptors.NordshriftConfigurationDescriptor;
import implementations.one.nordshrift.descriptors.NordshriftInstanceDescriptor;
import implementations.one.nordshrift.descriptors.NordshriftListDescriptor;
import implementations.one.system.System;

public class Nordshrift extends NordshriftSystem
{
    public static final String quickname = "$";

    public static final String name = "nordshrift";

    //

    public NordshriftListDescriptor descriptor001;

    public NordshriftInstanceDescriptor descriptor002;

    public NordshriftConfigurationDescriptor descriptor003;

    //

    public Nordshrift nordshrift001 = this;

    public Nordshrift nordshrift002 = this;

    public Nordshrift nordshrift003 = this;

    //

    public Nordshrift()
    {
        System.list(this);
    }
}