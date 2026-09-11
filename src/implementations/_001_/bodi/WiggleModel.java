package implementations._001_.bodi;

/**
 * Native structural hypotheses that Wiggle can infer from XML.
 *
 * These models describe observable structure; they do not assert application
 * semantics or authorize a Bodi change.
 */
public enum WiggleModel
{
    TREE("parent/child hierarchy and nesting"),
    SEQUENCE("ordered elements and structural progression"),
    COLLECTION("repeated sibling elements forming a set or list"),
    RECORD("bounded named fields or attributes describing an object"),
    REFERENCE("identifiers, keys, URIs, or links to another structure"),
    STATE("status, phase, mode, or lifecycle information"),
    COMMAND("operation-like structure with target and datum"),
    DESCRIPTION("metadata or schema-like structure describing another structure"),
    RELATION("structure connecting two or more identifiable objects"),
    STREAM("ordered recurring events or messages");

    private final String description;

    WiggleModel(String description)
    {
        this.description = description;
    }

    public String description()
    {
        return description;
    }
}
