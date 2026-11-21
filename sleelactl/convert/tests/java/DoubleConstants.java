// Double and Float constants (MIN_VALUE, infinities, NaN) and Double.toString of subnormal values
// (JLS 4.2.3; Double.toString gives the shortest decimal that reads back to the same double), and
// StringBuilder/StringBuffer.append of a double or float (append(String.valueOf(x))), and floating
// negation, which flips the sign (JLS 15.15.4: -0.0 for 0.0, not 0.0 - x).
public class DoubleConstants {
    public static void main(String[] args) {
        System.out.println(Double.MAX_VALUE + " " + Double.MIN_VALUE + " " + Double.MIN_NORMAL);
        System.out.println(Double.POSITIVE_INFINITY + " " + Double.NEGATIVE_INFINITY + " " + Double.NaN);
        System.out.println((Double.MAX_VALUE * 2 == Double.POSITIVE_INFINITY) + " " + (Double.NaN != Double.NaN));
        System.out.println(Double.MIN_VALUE / 2 + " " + Double.SIZE + " " + Double.PRECISION + " " + Double.MAX_EXPONENT);
        System.out.println(Float.MAX_VALUE + " " + Float.MIN_VALUE + " " + Float.MIN_NORMAL + " " + Float.POSITIVE_INFINITY);
        double[] subs = { 0x0.8p-1022, 2 * Double.MIN_VALUE, Double.MIN_NORMAL / 3, Double.MIN_VALUE * 12345,
                          0x0.fffffffffffffp-1022, 1.0e-310, 4.9e-324 * 3 };
        StringBuilder sb = new StringBuilder();
        for (double d : subs) sb.append(d).append(' ');
        System.out.println(sb);
        System.out.println(-Double.MIN_VALUE);
        StringBuilder t = new StringBuilder();
        float f = 0.1f;
        t.append(2.5).append(' ').append(3.0).append(' ').append(f).append(' ').append(1.0E10).append(' ').append(-0.0f);
        System.out.println(t);
        StringBuffer u = new StringBuffer();
        u.append(1.5f).append(Double.NaN).append(1e-5);
        System.out.println(u);
        double z = 0.0;
        float fz = 0.0f;
        double inf = Double.POSITIVE_INFINITY;
        System.out.println(-z + " " + -fz + " " + -(-z) + " " + -inf + " " + (1.0 / -z));
    }
}
