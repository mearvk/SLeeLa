// Enum constructors without modifiers, private, or with no arguments, in nested and top-level
// position, are recognized as constructors (the generated default is not added beside them).
public class EnumConstructors {
    private enum Hmac {
        A("x", 1), B("y", 2);
        private final String alg;
        private final int len;
        Hmac(String alg, int len) { this.alg = alg; this.len = len; }
    }
    enum Top {
        P, Q;
        private final int code;
        private Top() { code = 7; }
    }
    enum Unit {
        SEC("s");
        final String sym;
        private Unit(String sym) { this.sym = sym; }
    }
    public static void main(String[] a) {
        System.out.println(Hmac.B.alg + Hmac.B.len);
        System.out.println(Top.Q + " " + Top.Q.code + " " + Top.Q.ordinal());
        System.out.println(Unit.SEC.sym);
    }
}
