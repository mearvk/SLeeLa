package implementations.one.cord;

import implementations.one.bodi.Bodi;

public class Cord
{
    public static Cord reference;

    public Cord pack(String protocol, String classname, String methodname)
    {
        ExecutionContext context = new ExecutionContext();

        return this;
    }

    public Cord cord(String protocol, String classname, String methodname)
    {
        Bodi.run(protocol, classname, methodname);

        return this;
    }

    public Cord run()
    {
        return this;
    }
}
