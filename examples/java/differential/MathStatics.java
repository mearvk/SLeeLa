/** Java side of the MathStatics differential test; prints one result per line.
 *  Doubles are printed as q(x) = round(x * 1e6) so a last-bit difference in a
 *  transcendental does not mask a real divergence. */
public class MathStatics {
    static long q(double x) { return Math.round(x * 1000000.0); }
    public static void main(String[] args) {
        System.out.println(Math.abs(-7) + " " + Math.abs(-5000000000L) + " " + q(Math.abs(-2.5)) + " " + q(Math.abs(-0.0)) + " " + StrictMath.abs(-9L));
        System.out.println(Math.max(3, 9) + " " + Math.min(3, 9) + " " + q(Math.max(2.5, -1.0)) + " " + q(Math.min(2.5, -1.0)) + " " + StrictMath.max(4L, 8L) + " " + q(StrictMath.min(4.5, 8.5)));
        System.out.println(q(Math.floor(-2.5)) + " " + q(Math.ceil(2.1)) + " " + q(Math.ceil(-0.5)) + " " + q(Math.rint(2.5)) + " " + q(Math.rint(3.5)) + " " + q(Math.rint(-2.5)) + " " + q(StrictMath.floor(7.9)));
        System.out.println(Math.round(2.5) + " " + Math.round(-2.5) + " " + Math.round(2.4) + " " + Math.round(-2.6) + " " + StrictMath.round(0.5));
        System.out.println(q(Math.signum(-3.0)) + " " + q(Math.signum(0.0)) + " " + q(Math.signum(12.5)) + " " + q(Math.copySign(3.0, -1.0)) + " " + q(Math.copySign(-3.0, 1.0)));
        System.out.println(Math.floorDiv(-7, 2) + " " + Math.floorDiv(7, -2) + " " + Math.floorMod(-7, 3) + " " + Math.floorMod(7, -3) + " " + Math.ceilDiv(7, 2) + " " + Math.ceilDiv(-7, 2) + " " + Math.ceilMod(7, 2) + " " + Math.ceilMod(-7, 2));
        System.out.println(Math.clamp(15L, 0, 10) + " " + Math.clamp(-5L, 0, 10) + " " + Math.clamp(5L, 0, 10) + " " + q(Math.clamp(1.5, 0.0, 1.0)));
        System.out.println(Math.addExact(2000000000L, 2000000000L) + " " + Math.subtractExact(5L, 9L) + " " + Math.multiplyExact(123456L, 654321L) + " " + Math.negateExact(5L) + " " + Math.toIntExact(42L));
        System.out.println(Math.multiplyHigh(9223372036854775807L, 4L) + " " + Math.multiplyHigh(-9223372036854775807L, 4L) + " " + Math.multiplyFull(2000000000, 3));
        System.out.println(Math.powExact(3, 4) + " " + Math.powExact(2L, 62) + " " + Math.powExact(-2L, 63) + " " + Math.powExact(7, 0) + " " + StrictMath.powExact(5, 3));
        System.out.println(Math.unsignedMultiplyHigh(-1L, -1L) + " " + Math.unsignedMultiplyHigh(-1L, 2L) + " " + Math.unsignedMultiplyHigh(4294967296L, 4294967296L) + " " + Math.unsignedMultiplyHigh(3L, 5L));
        System.out.println(Math.unsignedMultiplyExact(-1L, 1L) + " " + Math.unsignedMultiplyExact(4294967296L, 2147483648L) + " " + Math.unsignedPowExact(2L, 63) + " " + Math.unsignedPowExact(3L, 3));
        System.out.println(Math.getExponent(1024.0) + " " + Math.getExponent(0.1) + " " + Math.getExponent(0.0) + " " + q((Math.ulp(1.0) * 4503599627370496.0)) + " " + q(((Math.nextUp(1.0) - 1.0) * 4503599627370496.0)) + " " + q(((1.0 - Math.nextDown(1.0)) * 9007199254740992.0)) + " " + q(((Math.nextAfter(1.0, 2.0) - 1.0) * 4503599627370496.0)) + " " + q(Math.scalb(3.0, 4)));
        System.out.println(q(Math.toDegrees(Math.PI)) + " " + q(Math.toRadians(180.0)) + " " + q(Math.E) + " " + q(Math.PI) + " " + q(Math.TAU) + " " + q(Math.hypot(3.0, 4.0)) + " " + q(Math.sqrt(144.0)) + " " + q(Math.cbrt(-27.0)));
        System.out.println(q(Math.pow(2.0, 10.0)) + " " + q(Math.pow(2.0, -2.0)) + " " + q(Math.pow(9.0, 0.5)) + " " + q(Math.exp(1.0)) + " " + q(Math.log(Math.E)) + " " + q(Math.sin(Math.PI / 2.0)) + " " + q(Math.cos(Math.PI)) + " " + q(Math.atan(1.0) * 4.0) + " " + q(Math.tanh(0.5)) + " " + q(Math.log10(1000.0)));
        System.out.println(ex(() -> Math.addExact(Long.MAX_VALUE, 1L)) + " " + ex(() -> Math.powExact(2L, 64)) + " " + ex(() -> Math.powExact(2L, -1)) + " " + ex(() -> Math.unsignedMultiplyExact(-1L, 2L)) + " " + ex(() -> Math.unsignedPowExact(2L, 64)));
    }
    static String ex(Runnable r) {
        try { r.run(); return "ok"; } catch (ArithmeticException e) { return "AE"; } catch (IllegalArgumentException e) { return "IAE"; }
    }
}
