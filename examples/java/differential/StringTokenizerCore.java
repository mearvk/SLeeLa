import java.util.NoSuchElementException;
import java.util.StringTokenizer;

/** Java side of the StringTokenizerCore differential test (java.util.StringTokenizer). */
public class StringTokenizerCore {
    static void run(String label, StringTokenizer t) {
        StringBuilder sb = new StringBuilder(label + " n=" + t.countTokens() + " :");
        while (t.hasMoreTokens()) { sb.append(" [").append(t.nextToken()).append("]"); }
        sb.append(" n=").append(t.countTokens()).append(" more=").append(t.hasMoreElements());
        System.out.println(sb);
    }
    public static void main(String[] args) {
        run("default", new StringTokenizer("the quick\tbrown\nfox\r\fjumps"));
        run("leading", new StringTokenizer("   a  b   "));
        run("empty", new StringTokenizer(""));
        run("onlydelims", new StringTokenizer("   \t "));
        run("comma", new StringTokenizer("a,b,,c,", ","));
        run("multi", new StringTokenizer("a=b;c=d", "=;"));
        run("retdelims", new StringTokenizer("a,b,,c,", ",", true));
        run("retdelims2", new StringTokenizer(",,x", ",", true));
        run("retdelims3", new StringTokenizer("1+2*3", "+*", true));
        run("nodelims", new StringTokenizer("abc", ""));
        run("single", new StringTokenizer("abc", "x"));

        StringTokenizer t = new StringTokenizer("a b,c d", " ");
        System.out.println(t.nextToken() + "|" + t.countTokens() + "|" + t.nextToken(",") + "|" + t.countTokens() + "|" + t.nextToken() + "|" + t.hasMoreTokens());
        StringTokenizer e = new StringTokenizer("p q");
        System.out.println(e.hasMoreElements() + " " + e.nextElement() + " " + e.nextElement() + " " + e.hasMoreElements());
        try { e.nextToken(); } catch (NoSuchElementException x) { System.out.println(x); }
        try { e.nextElement(); } catch (NoSuchElementException x) { System.out.println(x); }
        try { new StringTokenizer(null); } catch (NullPointerException x) { System.out.println("NPE"); }
        StringTokenizer nd = new StringTokenizer("abc", null);
        try { nd.hasMoreTokens(); } catch (NullPointerException x) { System.out.println("NPE delim"); }
        StringTokenizer sw = new StringTokenizer("x-y z", "-");
        System.out.println(sw.nextToken() + " " + sw.nextToken(" ") + " " + sw.nextToken("-"));
    }
}
