// Floating remainder (JLS 15.17.3): truncating, sign of the dividend, NaN for a zero divisor or
// an infinite dividend, the dividend for an infinite divisor or a zero dividend. The VM operator
// traps on a zero divisor, so the conversion goes through JavaOperands.remDouble.
public class FloatRemainder {
    public static void main(String[] args) {
        double z = 0.0, inf = 1.0 / z, nan = z / z;
        double[] as = { 5.5, -5.5, 5.5, -5.5, 1.0, inf, 2.0, -0.0, 0.0, 1e300, 7.0, nan, 0.1 };
        double[] bs = { 2.0, 2.0, -2.0, -2.0, z, 2.0, inf, 1.0, -3.0, 3.0, 2.5, 1.0, 0.03 };
        for (int i = 0; i < as.length; i++) System.out.println(as[i] % bs[i]);
        double x = 10.25;
        x %= 3.0;
        System.out.println(x);
        float f = 5.5f, fz = 0.0f;
        System.out.println(f % 2.0f);
        System.out.println(f % fz);
        System.out.println(-7.5f % 2.0f);
    }
}
