package implementations.one.nordshrift;

import implementations.one.system.System;

public class Nordshrift
{
    protected static Nordshrift prototype = new Nordshrift();

    //

    protected NordshriftExtender function;

    protected NordshriftExtender factor;

    protected NordshriftExtender parser;

    protected NordshriftExtender loader;

    //

    protected Nordshrift nordshrift = Nordshrift.prototype;

    protected Nordshrift reference = Nordshrift.prototype;

    //

    static
    {
        System.list(Nordshrift.class);
    }

    //

    private Nordshrift(Fantasy param)
    {
        System.list(this);
    }

    public Nordshrift()
    {
        System.list(this);
    }

    private class Fantasy {}
}

class NordshriftExtent extends Nordshrift
{

}

class NordshriftAtom extends Nordshrift
{

}