public class Varargs {
    static int sum(int... xs) { int s = 0; for (int x : xs) { s += x; } return s; }
    static String joinWith(String sep, String... parts) {
        String r = "";
        for (int i = 0; i < parts.length; i++) { if (i > 0) { r += sep; } r += parts[i]; }
        return r;
    }
    static double avg(double... v) { double t = 0; for (double d : v) { t += d; } return v.length == 0 ? 0 : t / v.length; }
    static int first(int a, int... rest) { return rest.length == 0 ? a : a + rest[0]; }
    int base;
    Varargs(int... parts) { base = sum(parts); }
    static void bump(double d) { System.out.println(d / 2); }
    public static void main(String[] args) {
        System.out.println(sum() + " " + sum(1) + " " + sum(1, 2, 3) + " " + sum(new int[]{4, 5}));
        System.out.println(joinWith(",") + "|" + joinWith(",", "a") + "|" + joinWith("-", "a", "b", "c"));
        System.out.println(avg(1, 2, 4) + " " + avg());
        System.out.println(first(5) + " " + first(5, 6, 7));
        System.out.println(new Varargs(1, 2, 3).base + " " + new Varargs().base);
        bump(3);
    }
}
