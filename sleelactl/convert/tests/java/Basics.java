public class Basics {
    static int fib(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); }
    static long mix(int a, int b) {
        int h = a * 31 + b;
        h ^= (h >>> 7);
        h += 17;
        byte bb = (byte) h;
        long r = (long) h * 1000003L;
        return r + bb;
    }
    public static void main(String[] args) {
        int sum = 0;
        for (int i = 0; i < 10; i++) { sum += i * i; }
        System.out.println("sum=" + sum);
        System.out.println(fib(15));
        System.out.println(mix(2147483647, 5));
        int big = 2147483647;
        big++;
        System.out.println(big);
        double d = 7 / 2;
        System.out.println(d);
        System.out.println(7.0 / 2);
        int[] a = {3, 1, 2};
        int t = 0;
        for (int x : a) t += x;
        System.out.println(t);
        String s = "abc";
        System.out.println(s.length() + 1);
        switch (t) { case 6: System.out.println("six"); break; default: System.out.println("other"); }
    }
}
