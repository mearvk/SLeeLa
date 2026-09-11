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

    public BodiChange(String reference, String method, String datum, String sequence)
    {
        this.reference = reference == null ? "" : reference;
        this.method = method == null ? "" : method;
        this.datum = datum == null ? "" : datum;
        this.sequence = sequence == null ? "" : sequence;
    }

    @Override
    public String toString()
    {
        return "BodiChange{" + reference + ", " + method + ", " + datum + ", " + sequence + "}";
    }
}
