import java.math.BigInteger;
import java.util.Arrays;
import java.util.Random;

/** Java side of the MathBigInteger differential test; prints one result per line. */
public class MathBigInteger {
    public static void main(String[] args) {
        BigInteger a = new BigInteger("123456789012345678901234567890");
        BigInteger b = new BigInteger("-987654321098765432109876543210987654321");
        BigInteger c = BigInteger.valueOf(-7);
        System.out.println(a.add(b) + " " + a.subtract(b) + " " + b.add(a.negate()) + " " + a.add(a.negate()));
        System.out.println(a.multiply(b) + " " + a.multiply(BigInteger.ZERO) + " " + c.multiply(c));
        System.out.println(b.divide(a) + " " + b.remainder(a) + " " + b.mod(a) + " " + a.divide(c) + " " + a.remainder(c) + " " + a.mod(BigInteger.valueOf(7)));
        BigInteger[] qr = b.divideAndRemainder(BigInteger.valueOf(1000000007));
        System.out.println(qr[0] + " " + qr[1]);
        System.out.println(BigInteger.TWO.pow(200) + " " + c.pow(3) + " " + a.gcd(b) + " " + BigInteger.valueOf(462).gcd(BigInteger.valueOf(-1071)));
        System.out.println(a.sqrt() + " " + BigInteger.TEN.pow(40).sqrt() + " " + Arrays.toString(BigInteger.valueOf(17).sqrtAndRemainder()));
        BigInteger m = new BigInteger("1000000000000000000000007");
        System.out.println(a.modPow(BigInteger.valueOf(65537), m) + " " + a.modInverse(m) + " " + BigInteger.valueOf(3).modPow(BigInteger.valueOf(-1), BigInteger.valueOf(11)));
        System.out.println(m.isProbablePrime(50) + " " + a.isProbablePrime(50) + " " + BigInteger.valueOf(561).isProbablePrime(50) + " " + BigInteger.valueOf(97).isProbablePrime(50) + " " + BigInteger.valueOf(1000000).nextProbablePrime() + " " + BigInteger.TWO.pow(89).subtract(BigInteger.ONE).isProbablePrime(50));
        System.out.println(a.compareTo(b) + " " + b.compareTo(a) + " " + a.equals(new BigInteger("123456789012345678901234567890")) + " " + a.min(b) + " " + a.max(b) + " " + a.signum() + b.signum() + BigInteger.ZERO.signum());
        System.out.println(a.and(b) + " " + a.or(b) + " " + a.xor(b) + " " + a.not() + " " + b.andNot(a) + " " + c.and(BigInteger.valueOf(0xff)) + " " + c.or(BigInteger.valueOf(8)));
        System.out.println(a.shiftLeft(70) + " " + b.shiftRight(37) + " " + c.shiftRight(1) + " " + c.shiftLeft(-2) + " " + a.testBit(3) + " " + b.testBit(200) + " " + c.setBit(0) + " " + c.clearBit(1) + " " + c.flipBit(5));
        System.out.println(a.bitLength() + " " + b.bitLength() + " " + c.bitLength() + " " + BigInteger.valueOf(-8).bitLength() + " " + a.bitCount() + " " + b.bitCount() + " " + c.bitCount() + " " + a.getLowestSetBit() + " " + BigInteger.valueOf(-48).getLowestSetBit());
        System.out.println(a.longValue() + " " + b.longValue() + " " + a.intValue() + " " + b.intValue() + " " + c.intValueExact() + " " + c.byteValue() + " " + a.shortValue());
        System.out.println(a.doubleValue() + " " + b.doubleValue() + " " + BigInteger.TWO.pow(1024).doubleValue() + " " + new BigInteger("9007199254740993").doubleValue() + " " + new BigInteger("9007199254740995").doubleValue() + " " + a.floatValue());
        System.out.println(a.hashCode() + " " + b.hashCode() + " " + c.hashCode() + " " + BigInteger.ZERO.hashCode());
        System.out.println(a.toString(16) + " " + b.toString(36) + " " + c.toString(2) + " " + new BigInteger("-zz", 36) + " " + new BigInteger("+00042"));
        System.out.println(Arrays.toString(a.toByteArray()) + " " + Arrays.toString(c.toByteArray()) + " " + Arrays.toString(BigInteger.valueOf(128).toByteArray()));
        System.out.println(new BigInteger(new byte[]{-1, 0, 1}) + " " + new BigInteger(1, new byte[]{-1, 0, 1}) + " " + new BigInteger(-1, new byte[]{1, 2}));
        System.out.println(new BigInteger(100, new Random(42)) + " " + new BigInteger(7, new Random(1)));
        System.out.println(BigInteger.valueOf(Long.MIN_VALUE) + " " + BigInteger.valueOf(Long.MIN_VALUE).longValue() + " " + BigInteger.valueOf(Long.MAX_VALUE).add(BigInteger.ONE).longValue());
        try { a.divide(BigInteger.ZERO); } catch (ArithmeticException e) { System.out.println(e.getMessage()); }
        try { new BigInteger("12x4"); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }
        try { new BigInteger("-00012345678901234x"); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }
        try { new BigInteger("1g", 16); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }
        try { new BigInteger("1x-2"); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }
        try { b.longValueExact(); } catch (ArithmeticException e) { System.out.println(e.getMessage()); }
        try { BigInteger.valueOf(4).modInverse(BigInteger.valueOf(8)); } catch (ArithmeticException e) { System.out.println(e.getMessage()); }
    }
}
