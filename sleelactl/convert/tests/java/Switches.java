public class Switches {
    static String name(int n) {
        switch (n) {
            case 1:
            case 2: return "low";
            case 3: return "three";
            default: return "many";
        }
    }
    static int fall(int n) {
        int r = 0;
        switch (n) {
            case 1: r += 1;
            case 2: r += 10;
            case 3: r += 100; break;
            default: r = -1;
            case 4: r += 1000;
        }
        return r;
    }
    static String arrow(String s) {
        switch (s) {
            case "a", "b" -> { return "ab"; }
            case "c" -> { return "c!"; }
            default -> { return "other"; }
        }
    }
    static int expr(int k) {
        int v = switch (k) {
            case 1 -> 10;
            case 2, 3 -> 20;
            default -> { int t = k * 2; yield t + 1; }
        };
        return v;
    }
    static String exprStr(char c) {
        return switch (c) { case 'x' -> "ex"; case 'y' -> "why"; default -> "zed"; };
    }
    public static void main(String[] args) {
        for (int i = 0; i < 5; i++) { System.out.println(name(i)); }
        for (int i = 1; i <= 5; i++) { System.out.println(fall(i)); }
        System.out.println(arrow("a") + arrow("b") + arrow("c") + arrow("zz"));
        for (int k = 0; k < 5; k++) { System.out.println(expr(k)); }
        System.out.println(exprStr('x') + exprStr('y') + exprStr('q'));
        int total = 0;
        for (int i = 0; i < 6; i++) {
            switch (i % 3) { case 0: continue; case 1: total += 1; break; default: total += 100; }
            total += 1000;
        }
        System.out.println(total);
        int x = 5;
        int y = (x = 7) + 1;
        System.out.println(x + " " + y);
        int i = 3;
        int j = i++ + ++i;
        System.out.println(i + " " + j);
        int k2 = 2147483647;
        int old = k2++;
        System.out.println(old + " " + k2);
        long L = 9223372036854775807L;
        long oldL = L++;
        System.out.println(oldL + " " + L);
        byte b = 127;
        byte ob = b++;
        System.out.println(ob + " " + b);
        int a1 = 0, a2 = 0;
        a1 = a2 = 9;
        System.out.println(a1 + a2);
        int[] arr = new int[3];
        int idx = 0;
        arr[idx++] = 5;
        arr[idx++] = 6;
        System.out.println(arr[0] + arr[1] + " " + idx);
    }
}
