package implementations.one.nordshrift;

import implementations.one.system.System;

public class Nordshrift
{
    static NordshriftExtender starter;

    //

    protected NordshriftExtender function;

    protected NordshriftExtender factor;

    protected NordshriftExtender parser;

    protected NordshriftExtender loader;

    //

    protected Nordshrift nordshrift = new Nordshrift();

    protected Nordshrift reference = new Nordshrift();

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
