public class Operands {
    static int div(int x, int y) { try { return x / y; } catch (ArithmeticException e) { return -999; } }
    static int rem(int x, int y) { try { return x % y; } catch (ArithmeticException e) { return -998; } }
    public static void main(String[] args) {
        int[] xs = {0, 1, -1, 7, -7, 2147483647, -2147483648, 65535, -32769, 123456789};
        int[] ys = {0, 1, -1, 3, 31, 32, 33, 2147483647, -2147483648};
        for (int x : xs) {
            for (int y : ys) {
                System.out.println("int " + x + " " + y + " " + (x + y) + " " + (x - y) + " " + (x * y) + " " + div(x, y) + " " + rem(x, y)
                    + " " + (x << y) + " " + (x >> y) + " " + (x >>> y) + " " + (x & y) + " " + (x | y) + " " + (x ^ y));
            }
            System.out.println("un " + x + " " + (-x) + " " + (~x) + " " + (byte) x + " " + (short) x + " " + (int) (char) x);
        }
        long[] ls = {0L, 1L, -1L, 9007199254740993L, 9223372036854775807L, -9223372036854775808L, 4294967296L, -4294967297L};
        for (long x : ls) {
            for (long y : ls) {
                System.out.println("long " + x + " " + y + " " + (x + y) + " " + (x - y) + " " + (x * y) + " " + (x & y) + " " + (x | y) + " " + (x ^ y));
            }
            System.out.println("l2 " + x + " " + (int) x + " " + (short) x + " " + (byte) x + " " + (int) (char) x + " " + (x << 3) + " " + (x >> 3) + " " + (x >>> 3));
        }
        double[] ds = {0.0, 1.5, -1.5, 2.5, 1e10, -1e10, 3e9, -3e9, 123456.789, 0.1};
        for (double d : ds) {
            System.out.println("d " + d + " " + (int) d + " " + (long) d + " " + (byte) d + " " + (short) d + " " + (d * 2) + " " + (d / 3) + " " + (d + 1));
        }
        byte b = 120; b += 10; System.out.println(b);
        short sh = 32760; sh += 10; System.out.println(sh);
        char c = 65530; c += 10; System.out.println((int) c);
        int i = 5; i *= 2.6; System.out.println(i);
        long lg = 1; lg <<= 40; System.out.println(lg);
        int k = 1; k <<= 33; System.out.println(k);
        int m = -17; m >>>= 28; System.out.println(m);
        int cnt = 0; cnt++; cnt++; cnt--; System.out.println(cnt);
        byte bc = 127; bc++; System.out.println(bc);
    }
}
