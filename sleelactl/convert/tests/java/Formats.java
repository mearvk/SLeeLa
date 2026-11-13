public class Formats {
    public static void main(String[] a) {
        System.out.println(String.format("%d|%5d|%-5d|%05d|%+d|%,d", 42, 42, 42, 42, 42, 1234567));
        System.out.println(String.format("%.2f|%8.3f|%-8.1f|%08.2f|%+.1f|%,.2f", 3.14159, 2.71828, 1.25, -3.5, 2.25, 1234567.891));
        System.out.println(String.format("%.0f %.1f %.2f %.3f", 0.5, 0.25, 0.125, 0.0005));
        System.out.println(String.format("%x %X %o %x", 255, 255, 8, -1));
        System.out.println(String.format("%s %10s %-10s| %.3s", "ab", "cd", "ef", "ghijkl"));
        System.out.println(String.format("%c %b %% ", 'x', true) + "end");
        long big = 9223372036854775807L;
        byte bt = -1;
        System.out.println(String.format("%d %x %x %x", big, big, bt, -big));
        double dv = 1234.5;
        float fv = 2.5f;
        int n = 7;
        System.out.println(String.format("n=%d dv=%.1f fv=%.2f s=%s", n, dv, fv, "t" + n));
        System.out.println(String.format("%6.2f%%", 12.345));
        System.out.println(String.format("[%s]", (Object) null == null ? "null" : "x"));
        System.out.println(String.valueOf(42) + String.valueOf(2.5) + String.valueOf(true) + String.valueOf('c') + String.valueOf(10L));
        System.out.println(String.join(",", "a", "b", "c") + "|" + String.join("-", new String[]{"x", "y"}) + "|" + String.join("/"));
        String name = null;
        System.out.println(String.valueOf(name) + " " + String.format("%s", name));
    }
}
