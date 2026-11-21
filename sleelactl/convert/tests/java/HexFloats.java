// Hexadecimal floating-point literals (JLS 3.10.2): a hex significand, an optional fraction and a
// binary exponent. The conversion writes the exact value as a 17-digit decimal spelling.
public class HexFloats {
    static final double MIN_NORMAL = 0x1.0p-1022;
    static final double MIN_VALUE = 0x0.0000000000001P-1022;
    static final double MAX = 0x1.fffffffffffffP+1023;
    static final double TWO54 = 0x1.0p54;
    static final double S1 = -0x1.5555555555549p-3;
    static final double BIG = 0x1.00000_ffff_ffffp500;

    public static void main(String[] args) {
        System.out.println(MIN_NORMAL);
        System.out.println(MIN_VALUE);
        System.out.println(MAX);
        System.out.println(TWO54);
        System.out.println(S1);
        System.out.println(BIG > 0x1.0p500);
        System.out.println(0x1p3 + 0xA.8p0);
        float f = 0x1.8p1f;
        System.out.println(f);
        System.out.println(0x1.fffffeP+127f);
        System.out.println(0x10);
        System.out.println(0x7fffffff);
        System.out.println(4.9e-324 + " " + (2 * 4.9e-324) + " " + -0x1p-1074);
        System.out.println(0x0.8p-1022 * 0x1p1022 + " " + (0x0.8p-1022 == 0x1p-1023) + " " + 0x0.0000000000001P-1022 * 0x1p1000 * 0x1p74);
    }
}
