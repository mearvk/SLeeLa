package implementations.one.nordshrift.extenders;

import implementations.one.parsers.NordshriftExtent;

public class NordshriftExtender extends NordshriftExtent
{
    NordshriftExtender extender;

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

    public NordshriftExtender()
    {

    }
}
