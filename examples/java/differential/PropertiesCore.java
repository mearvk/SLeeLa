import java.io.ByteArrayOutputStream;
import java.io.StringReader;
import java.io.StringWriter;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Properties;
import java.util.TreeSet;

/** Java side of the PropertiesCore differential test (java.util.Properties, System properties). */
public class PropertiesCore {
    static void names(Properties p) {
        TreeSet<String> s = new TreeSet<>();
        Enumeration<?> e = p.propertyNames();
        while (e.hasMoreElements()) { s.add((String) e.nextElement()); }
        for (String n : s) { System.out.println("name " + n); }
    }
    /** Prints text line by line; line number dateLine (the date comment, which differs per run) prints as #DATE. */
    static void dump(String text, int dateLine) {
        int line = 0;
        int from = 0;
        int nl = text.indexOf("\n", from);
        while (nl >= 0) {
            if (line == dateLine) { System.out.println("#DATE"); }
            else { System.out.println(text.substring(from, nl)); }
            line = line + 1;
            from = nl + 1;
            nl = text.indexOf("\n", from);
        }
    }
    public static void main(String[] args) throws Exception {
        Properties d = new Properties();
        d.setProperty("zeta", "from defaults");
        d.setProperty("shared", "default value");
        Properties p = new Properties(d);
        System.out.println(p.setProperty("shared", "own value"));
        System.out.println(p.setProperty("shared", "own value 2"));
        p.setProperty("b key", " lead=x:y#z!");
        p.setProperty("a", "1\\2");
        p.setProperty("tab", "t\tend");
        p.setProperty("nl", "line1\nline2\rline3\f.");
        p.setProperty("latin", "caf\u00e9");
        p.setProperty("euro", "\u20ac5");
        System.out.println(p.getProperty("zeta"));
        System.out.println(p.getProperty("shared"));
        System.out.println(p.getProperty("missing", "fallback"));
        System.out.println(p.getProperty("missing"));
        System.out.println(p.size() + " " + d.size());
        System.out.println(p.containsKey("zeta") + " " + p.containsKey("a"));
        System.out.println(p.stringPropertyNames().size());
        names(p);

        StringWriter sw = new StringWriter();
        p.store(sw, "hello\nworld\n!bang\r\n#hash");
        dump(sw.toString(), 4);

        ByteArrayOutputStream bo = new ByteArrayOutputStream();
        p.store(bo, null);
        byte[] b = bo.toByteArray();
        String bytes = "";
        int high = 0;
        for (int i = 0; i < b.length; i++) {
            int v = b[i] & 0xff;
            if (v > 126 || (v < 32 && v != 10)) { high = high + 1; }
            bytes = bytes + (char) v;
        }
        System.out.println("bytes " + b.length + " outside ASCII " + high);
        dump(bytes, 0);

        Properties r = new Properties();
        r.load(new StringReader("# comment\n! bang comment\n  key1 = value1\nkey2:value2\nkey3 value3\n"
            + "multi = one \\\n    two \\\n    three\nesc\\ aped = \\u0041\\t\\n.\nempty\n"));
        names(r);
        System.out.println("[" + r.getProperty("key1") + "][" + r.getProperty("key2") + "][" + r.getProperty("key3") + "]");
        System.out.println("[" + r.getProperty("multi") + "]");
        System.out.println("[" + r.getProperty("esc aped") + "] [" + r.getProperty("empty") + "]");

        Properties c = (Properties) p.clone();
        System.out.println(c.getProperty("zeta") + " " + c.size());
        System.out.println(c.remove("a", "nope") + " " + c.remove("a", "1\\2") + " " + c.containsKey("a") + " " + p.containsKey("a"));
        System.out.println(c.replace("tab", "wrong", "x") + " " + c.replace("tab", "t\tend", "replaced") + " " + c.getProperty("tab"));
        System.out.println(c.remove("nl") != null);
        System.out.println(c.size());

        System.out.println(System.getProperty("sleela.test.key"));
        System.out.println(System.setProperty("sleela.test.key", "v1"));
        System.out.println(System.setProperty("sleela.test.key", "v2"));
        System.out.println(System.getProperty("sleela.test.key"));
        System.out.println(System.getProperties().getProperty("sleela.test.key"));
        System.out.println(System.getProperty("sleela.test.missing", "dflt"));
        System.out.println(System.clearProperty("sleela.test.key"));
        System.out.println(System.getProperty("sleela.test.key"));
        System.out.println(System.getProperty("file.separator") + " " + System.getProperty("path.separator"));
        try { System.getProperty(""); } catch (IllegalArgumentException e) { System.out.println("IAE " + e.getMessage()); }
        try { System.getProperty(null); } catch (NullPointerException e) { System.out.println("NPE " + e.getMessage()); }
    }
}
