package implementations._001_.bodi;

/** No-dependency smoke test for the Bodi model and XML boundary. */
public final class BodiSmokeTest
{
    private BodiSmokeTest() { }

    public static void main(String[] args) throws Exception
    {
        String xml = "<bodi system=\"xxx\" method=\"install\" sequence=\"001\" " +
            "starter=\"test\" man=\"test-user\"><datum>yyy</datum></bodi>";
        BodiChange change = BodiXmlDocument.parse(xml);

        if (!"xxx".equals(change.reference)) throw new AssertionError("reference");
        if (!"install".equals(change.method)) throw new AssertionError("method");
        if (!"001".equals(change.sequence)) throw new AssertionError("sequence");
        if (!"yyy".equals(change.datum)) throw new AssertionError("datum");
        if (!"test".equals(change.starter)) throw new AssertionError("starter");
        if (!"test-user".equals(change.man)) throw new AssertionError("man");

        BodiWitness witness = new BodiWitness(change.reference, change.reference,
            "object-change", change.method, change.sequence,
            change.starter, change.man, java.util.Arrays.asList("xxx", "001", "install"));
        if (!witness.path.contains("install")) throw new AssertionError("witness path");

        System.out.println("Bodi smoke test passed");
    }
}
