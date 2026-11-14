package qualify;

/** Own methods named like SLeeLa built-ins (the qualify rows of conversion.model), called unqualified. */
public class Qualify {
    int base = 10;
    int bigadd(int a, int b) { return a + b + base; }
    static String timeJson() { return "own timeJson"; }
    boolean osExists(String p) { return p.length() > 3; }
    int run() { return bigadd(1, 2); }
    public static void main(String[] args) {
        Qualify q = new Qualify();
        System.out.println(q.run() + " " + timeJson() + " " + q.osExists("abcd"));
    }
}
