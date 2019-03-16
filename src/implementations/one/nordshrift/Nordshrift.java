package implementations.one.nordshrift;

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