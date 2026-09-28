package implementations._001_.nordshrift.descriptors;

import implementations._001_.nordshrift.Nordshrift;
import implementations._001_.nordshrift.NordshriftMonitor;
import implementations._001_.nordshrift.NordshriftSystem;

import java.lang.reflect.Field;

public class NordshriftInstanceDescriptor
{
    public NordshriftInstancingList list001;

    //

    public NordshriftInstancer nordshriftinstancer;

    public NordshriftSystemInstancer nordshriftsysteminstancer;

    public NordshriftMonitorInstancer nordshriftmonitorinstancer;

    public NordshriftExtenderInstancer nordshriftextenderinstancer;

    //

    public NordshriftInstanceDescriptor()
    {
        this.nordshriftinstancer = new NordshriftInstancer();

        this.nordshriftsysteminstancer = new NordshriftSystemInstancer();

        this.nordshriftmonitorinstancer = new NordshriftMonitorInstancer();

        this.nordshriftextenderinstancer = new NordshriftExtenderInstancer();
    }

    //

    class NordshriftInstancer implements Runnable
    {
        private Integer instances = 0;

        public NordshriftInstancer()
        {
            for(Field field : Nordshrift.class.getDeclaredFields())
            {
                if(field.getDeclaringClass().isInstance(Nordshrift.class))
                {
                    instances++;
                }
            }
        }

        @Override
        public void run()
        {
            if( instances==4 )
            {
                try
                {
                    Buildx.name("nordshrift").getDeclaredField("nordshrift001").newInstance();

                    Buildx.name("nordshrift").getDeclaredField("nordshrift002").newInstance();

                    Buildx.name("nordshrift").getDeclaredField("nordshrift003").newInstance();

                    Buildx.name("nordshrift").getDeclaredField("nordshrift004").newInstance();
                }
                catch (Exception exception)
                {
                    System.out.println(exception);
                }
            }
        }
    }

    class NordshriftSystemInstancer implements Runnable
    {
        private Integer instances = 0;

        public NordshriftSystemInstancer()
        {
            for(Field field : Nordshrift.class.getDeclaredFields())
            {
                if(field.getDeclaringClass().isInstance(NordshriftSystem.class))
                {
                    instances++;
                }
            }
        }

        @Override
        public void run()
        {
            if( instances==1 )
            {
                try
                {
                    Buildx.name("nordshrift").getDeclaredField("nordshriftsystem001").newInstance();
                }
                catch (Exception exception)
                {
                    System.out.println(exception);
                }
            }
        }
    }

    class NordshriftMonitorInstancer implements Runnable
    {
        private Integer instances = 0;

        public NordshriftMonitorInstancer()
        {
            for(Field field : Nordshrift.class.getDeclaredFields())
            {
                if(field.getDeclaringClass().isInstance(NordshriftMonitor.class))
                {
                    instances++;
                }
            }
        }

        @Override
        public void run()
        {
            try
            {
                Buildx.name("nordshrift").getDeclaredField("nordshriftsystem001").getDeclaredField("nordshriftsystem001").newInstance();
            }
            catch (Exception exception)
            {
                System.out.println(exception);
            }
        }
    }

    class NordshriftExtenderInstancer implements Runnable
    {
        private Integer instances = 0;

        @Override
        public void run()
        {
            try
            {
                Buildx.name("nordshrift").getDeclaredField("nordshriftsystem001").getDeclaredField("nordshriftsystem001").newInstance();
            }
            catch (Exception exception)
            {
                System.out.println(exception);
            }
        }
    }

    static class Buildx
    {
        static Buildx name(String name){return null;}

        static Buildx instance(String name){return null;}

        Buildx getDeclaredField(String name){return null;}

        Buildx newInstance(){return null;}
    }
}

