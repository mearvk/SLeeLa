public class OverloadWidening {
    static String pick(int x) { return "int"; }
    static String pick(long x) { return "long"; }
    static String pick(double x) { return "double"; }
    static String fixed(String x) { return "fixed"; }
    static String fixed(String... x) { return "varargs"; }
    public static void main(String[] args) {
        byte b = 1;
        System.out.println(pick(b) + " " + pick(1L) + " " + fixed("x"));
    }
}
