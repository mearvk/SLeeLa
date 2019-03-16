package implementations._001.nordshrift;

public class Nordshrift
{
    public NordshriftExtender extender;

    public NordshriftExtender factory;

    public NordshriftExtender parser;

    public NordshriftExtender loader;

    public Nordshrift nordshrift;

    public Nordshrift reference;

    //

    static
    {
        Nordshrift.starter = new NordshriftStarter();
    }

    //

    public static NordshriftExtender starter;
}
