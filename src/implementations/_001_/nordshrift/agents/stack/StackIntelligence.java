package implementations._001_.nordshrift.agents.stack;

import java.lang.instrument.Instrumentation;

public class StackIntelligence extends StackIntelligenceExtent
{
    static
    {
        System.out.println("[nordshrift] stackintelligence™");
    }

    public static void premain(String args, Instrumentation inst)
    {
        StackIntelligence stackintelligence = new StackIntelligence();
    }
}

class StackIntelligenceExtent
{
    public StackIntelligenceExtent()
    {

    }
}

class LoadDescriptor
{

}

class CircuitDescriptor
{

}

class LoadPolicy
{

}

class CircuitPolicy
{

}

class StackTrace
{
    public StackTraceElement[] elements;


}