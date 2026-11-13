public class Control {
    static String name(int n) {
        switch (n) {
            case 1:
            case 2: return "low";
            case 3: return "three";
            default: return "many";
        }
    }
    static int sw(String s) {
        int r = 0;
        switch (s) {
            case "a": r = 1; break;
            case "b": r = 2; break;
            default: r = -1;
        }
        return r;
    }
    static int collatz(int n) {
        int steps = 0;
        while (n != 1) {
            if (n % 2 == 0) { n = n / 2; } else { n = 3 * n + 1; }
            steps++;
        }
        return steps;
    }
    static int risky(int x) {
        int r = 0;
        try {
            if (x < 0) { throw new IllegalStateException("neg"); }
            r = 100 / x;
        } catch (ArithmeticException e) {
            r = -1;
        } catch (IllegalStateException e) {
            r = -2;
        } finally {
            System.out.println("finally " + x);
        }
        return r;
    }
    public static void main(String[] args) {
        for (int i = 0; i < 5; i++) { System.out.println(name(i)); }
        System.out.println(sw("a") + " " + sw("b") + " " + sw("zzz"));
        System.out.println(collatz(27));
        System.out.println(risky(5) + " " + risky(0) + " " + risky(-3));
        int total = 0;
        for (int i = 0; i < 10; i++) {
            if (i % 2 == 0) continue;
            if (i > 7) break;
            total += i;
        }
        System.out.println(total);
        int n = 0;
        do { n += 3; } while (n < 10);
        System.out.println(n);
        boolean flag = n > 5 && total < 100 || false;
        System.out.println(flag ? "yes" : "no");
        int x = 10, y = 3;
        System.out.println(x / y + " " + x % y + " " + (double) x / y);
        long big = 1;
        for (int i = 0; i < 40; i++) { big *= 3; }
        System.out.println(big);
        int h = 7;
        for (int i = 0; i < 100; i++) { h = h * 31 + i; }
        System.out.println(h);
    }
}
