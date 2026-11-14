package model;

/**
 * The laws of impl/subjects/conversion/conversion.model that are not int + - *:
 * narrowing casts, double to integral casts, long division and shifts, unary
 * minus, compound assignment and increments on narrow types, and char as text.
 */
public class ModelDemo {
    public static void main(String[] args) {
        int big = 200;
        int neg = -129;
        System.out.println((byte) big + " " + (byte) neg + " " + (short) 70000 + " " + (int) (char) 65601);
        long L = 3000000000L;
        System.out.println((int) L + " " + (byte) L + " " + (short) L);
        double d = 3.99;
        double nd = -3.99;
        double huge = 1e20;
        double nan = 0.0 / 0.0;
        System.out.println((int) d + " " + (int) nd + " " + (int) huge + " " + (int) nan + " " + (long) huge + " " + (long) -huge);
        System.out.println((byte) 300.7 + " " + (short) -40000.5 + " " + (int) (char) 66.9);
        long a = 7L;
        long b = -2L;
        System.out.println(a / b + " " + a % b + " " + Long.MIN_VALUE / -1L);
        try { long z = 0L; System.out.println(a / z); } catch (ArithmeticException e) { System.out.println("long / 0: " + e.getMessage()); }
        try { long z = 0L; System.out.println(a % z); } catch (ArithmeticException e) { System.out.println("long % 0: " + e.getMessage()); }
        System.out.println((1L << 65) + " " + (-16L >> 66) + " " + (-16L >>> 60) + " " + (1L << 63));
        int m = Integer.MIN_VALUE;
        System.out.println(-m + " " + (-(-m)));
        byte bb = 120;
        bb += 10;
        short ss = 32767;
        ss++;
        char cc = 'z';
        cc += 1;
        int ci = cc;
        System.out.println(bb + " " + ss + " " + ci);
        byte bi = 127;
        bi++;
        char cz = 65535;
        cz++;
        int czi = cz;
        System.out.println(bi + " " + czi);
        StringBuilder sb = new StringBuilder();
        char x = 'Q';
        sb.append(x).append('!').append(65);
        sb.insert(0, 'A');
        System.out.println(sb.toString());
    }
}
