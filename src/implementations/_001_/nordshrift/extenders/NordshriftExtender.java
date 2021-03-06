package implementations._001_.nordshrift.extenders;

import implementations._001_.nordshrift.NordshriftSystem;
import implementations._001_.parsers.NordshriftExtent;

public class NordshriftExtender extends NordshriftExtent
{
    public NordshriftExtender extender;

    public String type;

    public NordshriftSystem system;

    //

    public NordshriftExtender(NordshriftSystem system)
    {
        this.system = system;
    }

    public NordshriftExtender(String type)
    {
        try
        {
            Class klass = Class.forName(type);

            if(klass.isInstance(NordshriftExtender.class))
            {
                this.extender = (NordshriftExtender)klass.newInstance();
            }
        }
        catch (Exception exception)
        {

        }
    }

    public NordshriftExtender(NordshriftSystem system, String type)
    {
        try
        {
            Class klass = Class.forName(type);

            if(klass.isInstance(NordshriftExtender.class))
            {
                this.extender = (NordshriftExtender)klass.newInstance();
            }
        }
        catch (Exception exception)
        {

        }
    }

    public NordshriftExtender()
    {

    }

    //

    public NordshriftExtender cast(String classname)
    {
        return this;
    }

    public NordshriftExtender method(String methodname)
    {
        return this;
    }

    public NordshriftExtender params(Object...objects)
    {
        return this;
    }

    public NordshriftExtender exec()
    {
        return this;
    }
}
