package implementations.one.cord;

import implementations.one.bodi.Bodi;

public class Cord
{
    public static Cord reference;

    //

    public Cord cold = new CordExtender(this);

    public Cord hot = new CordExtender(this);

    //

    public Cord pack()
    {
        return this;
    }

    public Cord push(String protocol, String classname, String methodname)
    {
        Bodi.run(protocol, classname, methodname);

        return this;
    }

    public Cord push(String classname, String methodname)
    {
        Bodi.run("{bodi}", classname, methodname);

        return this;
    }

    public Cord run()
    {
        return this;
    }

    public Cord defer()
    {
        return this;
    }
}

class CordExtender extends Cord
{
    public CordExtender(Cord cord)
    {

    }
}