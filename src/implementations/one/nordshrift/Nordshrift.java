package implementations.one.nordshrift;

import implementations.one.parsers.NordshriftExtent;
import implementations.one.parsers.NordshriftParser;
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
    public NordshriftExtender functionary   = new NordshriftFunctionary();

    public NordshriftExtender factory       = new NordshriftFactory();

    public NordshriftExtender parser        = new NordshriftParser();

    public NordshriftExtender loader        = new NordshriftLoader();
}