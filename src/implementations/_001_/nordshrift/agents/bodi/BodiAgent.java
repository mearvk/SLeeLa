package implementations._001_.nordshrift.agents.bodi;

import java.lang.instrument.Instrumentation;
import java.util.ArrayList;

public class BodiAgent extends BodiAgentExtent
{
    static
    {
        System.out.println("[nordshrift] Bodi Intelligence™");
    }

    public static void premain(String args, Instrumentation inst)
    {
        BodiAgent agent = new BodiAgent();
    }
}

class BodiAgentExtent
{
    public ArrayList<Pattern> elements = new ArrayList<Pattern>();

    public BodiAgentExtent()
    {

    }
}

class Pattern
{
    public StackTraceElement[] elements;
}