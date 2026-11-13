/** Java side of the OperandSemantics differential test; prints one result per line. */
public class OperandSemantics {
    static String div(int x, int y) { try { return "" + (x / y); } catch (ArithmeticException e) { return "AE"; } }
    static String rem(int x, int y) { try { return "" + (x % y); } catch (ArithmeticException e) { return "AE"; } }
    static String divL(long x, long y) { try { return "" + (x / y); } catch (ArithmeticException e) { return "AE"; } }
    static String remL(long x, long y) { try { return "" + (x % y); } catch (ArithmeticException e) { return "AE"; } }
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
        long[] ns = {0L, 1L, -1L, 3L, 63L, 64L, 65L, -9223372036854775808L};
        for (long x : ls) {
            for (long y : ns) {
                System.out.println("long " + x + " " + y + " " + (x + y) + " " + (x - y) + " " + (x * y) + " " + divL(x, y) + " " + remL(x, y)
                    + " " + (x << y) + " " + (x >> y) + " " + (x >>> y));
            }
            System.out.println("l2 " + x + " " + (int) x + " " + (short) x + " " + (byte) x + " " + (int) (char) x);
        }
        double[] ds = {0.0 / 0.0, 1.0 / 0.0, -1.0 / 0.0, 3.99, -3.99, 1e10, -1e10, 2147483647.5, -2147483648.5, 9.3e18, -9.3e18, 1e300, 70000.9, -129.5, 65536.0};
        for (double d : ds) {
            System.out.println("dbl " + (int) d + " " + (long) d + " " + (short) d + " " + (byte) d + " " + (int) (char) d);
        }
        // typed locals and fields wrap at their declared width
        byte b = 127; b = (byte) (b + 1); short sh = 32767; sh = (short) (sh + 1); char ch = 65535; ch = (char) (ch + 1);
        int i = 2147483647; i = i + 1;
        System.out.println("typed " + b + " " + sh + " " + (int) ch + " " + i);
        byte acc = 0;
        for (int k = 0; k < 300; k++) { acc = (byte) (acc + 1); }
        int h = 0;
        for (int k = 0; k < 40; k++) { h = 31 * h + (k * 7919); }
        System.out.println("loop " + acc + " " + h);
    }
}
