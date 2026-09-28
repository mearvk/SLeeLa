package implementations._001_.bodi;

import java.rmi.Remote;

/**
 * Fluent address for a known Bodi system. The API deliberately reads like
 * Bodi.system("name").propagative("001").install("network_witness").
 */
public final class BodiObjectReference
{
    private final BodiExtender extender;
    private final String system;
    private String propagation;

    BodiObjectReference(BodiExtender extender, String system)
    {
        this.extender = extender;
        this.system = system == null ? "" : system;
    }

    public BodiObjectReference propagative(String sequence)
    {
        this.propagation = sequence == null ? "" : sequence;
        return this;
    }

    public Remote resolve() throws Exception
    {
        return extender.resolveSystem(system);
    }

    public BodiWitness witness(String method, String datum, String starter, String man)
    {
        return extender.witness(system, system, "object-change", method,
            propagation, starter, man);
    }

    public Object install(String datum) throws Exception
    {
        return install(datum, "bodi", "unknown");
    }

    public Object install(String datum, String starter, String man) throws Exception
    {
        return extender.invoke(new BodiChange(system, "install", datum, propagation),
            starter, man);
    }
}
