package implementations.one.nordshrift.drivers;

import implementations.one.nordshrift.Nordshrift;

import implementations.one.nordshrift.extenders.NordshriftFactorial;
import implementations.one.nordshrift.extenders.NordshriftFunctional;
import implementations.one.nordshrift.extenders.NordshriftLoader;
import implementations.one.nordshrift.extenders.NordshriftStarter;

import implementations.one.nordshrift.monitors.NordshriftSystemRuntimeMonitor;
import implementations.one.nordshrift.monitors.NordshriftSystemShutdownMonitor;
import implementations.one.nordshrift.monitors.NordshriftSystemStartupMonitor;

public class NordshriftDriver
{
    //TODO: COMPLETE WITH XML LOOKUP ONLY
    public NordshriftDriver()
    {
        //System.list(this);

        try
        {
            Nordshrift nordshrift = (Nordshrift)Class.forName("implementations.one.nordshrift.Nordshrift").newInstance();

            //

            nordshrift.extender001.extender = (NordshriftStarter)Class.forName("implementations.one.nordshrift.extenders.NordshriftStarter").newInstance();

            nordshrift.extender002.extender = (NordshriftLoader)Class.forName("implementations.one.nordshrift.extenders.NordshriftLoader").newInstance();

            nordshrift.extender003.extender = (NordshriftFunctional)Class.forName("implementations.one.nordshrift.extenders.NordshriftFunctional").newInstance();

            nordshrift.extender004.extender = (NordshriftFactorial)Class.forName("implementations.one.nordshrift.extenders.NordshriftFactorial").newInstance();

            //

            nordshrift.extender001.type = nordshrift.extender001.extender.getClass().getSimpleName();

            nordshrift.extender002.type = nordshrift.extender002.extender.getClass().getSimpleName();

            nordshrift.extender003.type = nordshrift.extender003.extender.getClass().getSimpleName();

            nordshrift.extender004.type = nordshrift.extender004.extender.getClass().getSimpleName();

            //

            nordshrift.monitor001.thread = (NordshriftSystemStartupMonitor)Class.forName("implementations.one.nordshrift.monitors.NordshriftSystemStartupMonitor").newInstance();

            nordshrift.monitor002.thread = (NordshriftSystemRuntimeMonitor)Class.forName("implementations.one.nordshrift.monitors.NordshriftSystemRuntimeMonitor").newInstance();

            nordshrift.monitor003.thread = (NordshriftSystemShutdownMonitor)Class.forName("implementations.one.nordshrift.monitors.NordshriftSystemShutdownMonitor").newInstance();

            //

            nordshrift.monitor001.type = nordshrift.monitor001.thread.getClass().getSimpleName();

            nordshrift.monitor002.type = nordshrift.monitor002.thread.getClass().getSimpleName();

            nordshrift.monitor003.type = nordshrift.monitor003.thread.getClass().getSimpleName();
        }
        catch (Exception exception)
        {
            System.exit(0);
        }
    }
}
