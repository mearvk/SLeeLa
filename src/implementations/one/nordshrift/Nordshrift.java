package implementations.one.nordshrift;

import implementations.one.nordshrift.extenders.NordshriftExtender;
import implementations.one.system.System;

public class Nordshrift extends NordshriftSystem
{
    public String name = "$";

    //

    public Nordshrift nordshrift = this;

    public Nordshrift reference = this;

    //

    static
    {
        System.list(Nordshrift.class);
    }

    //

    public Nordshrift()
    {
        System.list(this);
    }
}

class NordshriftSystem
{
    public NordshriftExtender extender001 = new NordshriftExtender("extenders.Functional");

    public NordshriftExtender extender002 = new NordshriftExtender("extenders.Factorial");

    public NordshriftExtender extender003 = new NordshriftExtender("extenders.Loader");

    public NordshriftExtender extender004 = new NordshriftExtender("extenders.Starter");

    //

    public void init()
    {

    }
}