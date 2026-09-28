package implementations._001_.bodi;

import java.io.Serializable;

/** Describes one intended operation against a Bodi system object. */
public final class BodiChange implements Serializable
{
    private static final long serialVersionUID = 1L;

    public final String reference;
    public final String method;
    public final String datum;
    public final String sequence;
    public final String starter;
    public final String man;

    public BodiChange(String reference, String method, String datum, String sequence)
    {
        this(reference, method, datum, sequence, "", "");
    }

    public BodiChange(String reference, String method, String datum, String sequence,
                      String starter, String man)
    {
        this.reference = value(reference);
        this.method = value(method);
        this.datum = value(datum);
        this.sequence = value(sequence);
        this.starter = value(starter);
        this.man = value(man);
    }

    private static String value(String value)
    {
        return value == null ? "" : value;
    }

    @Override
    public String toString()
    {
        return "BodiChange{" + reference + ", " + method + ", " + datum + ", " +
            sequence + ", starter=" + starter + ", man=" + man + "}";
    }
}
