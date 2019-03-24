package implementations._001_.nordshrift.agents.stack;

import java.lang.instrument.Instrumentation;
import java.util.ArrayList;

public class StackAgent extends StackAgentExtent
{
    static
    {
        System.out.println("[nordshrift] Stack Intelligence™");
    }

    public static void premain(String args, Instrumentation inst)
    {
        StackAgent agent = new StackAgent();
    }
}

class StackAgentExtent
{
    public ArrayList<Pattern> elements = new ArrayList<Pattern>();

    public StackAgentExtent()
    {

    }
}

class Pattern
{
    public StackTraceElement[] elements;
}