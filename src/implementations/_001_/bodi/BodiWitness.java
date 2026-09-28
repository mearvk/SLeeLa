package implementations._001_.bodi;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Immutable-ish witness record describing a requested object change.
 * A witness is evidence of intent and execution, not a replacement for the object.
 */
public final class BodiWitness implements Serializable
{
    private static final long serialVersionUID = 1L;

    public final String system;
    public final String reference;
    public final String change;
    public final String method;
    public final String sequence;
    public final String starter;
    public final String man;
    public final long createdAt;
    public final List<String> path;

    public BodiWitness(String system, String reference, String change, String method,
                       String sequence, String starter, String man, List<String> path)
    {
        this.system = value(system);
        this.reference = value(reference);
        this.change = value(change);
        this.method = value(method);
        this.sequence = value(sequence);
        this.starter = value(starter);
        this.man = value(man);
        this.createdAt = System.currentTimeMillis();
        this.path = Collections.unmodifiableList(new ArrayList<String>(path == null
            ? Collections.<String>emptyList() : path));
    }

    private static String value(String value)
    {
        return value == null ? "" : value;
    }

    @Override
    public String toString()
    {
        return "BodiWitness{" +
            "system='" + system + '\'' +
            ", reference='" + reference + '\'' +
            ", change='" + change + '\'' +
            ", method='" + method + '\'' +
            ", sequence='" + sequence + '\'' +
            ", starter='" + starter + '\'' +
            ", man='" + man + '\'' +
            ", path=" + path +
            '}';
    }
}
