/** Java side of the IntegerStatics differential test; prints one result per line. */
public class IntegerStatics {
    public static void main(String[] args) {
        System.out.println(Integer.parseInt("42") + " " + Integer.parseInt("-123") + " " + Integer.parseInt("ff", 16) + " " + Integer.parseInt("-1010", 2) + " " + Integer.parseUnsignedInt("4294967295"));
        System.out.println(Integer.toString(255) + " " + Integer.toString(-255, 16) + " " + Integer.toBinaryString(10) + " " + Integer.toBinaryString(-1) + " " + Integer.toHexString(-255) + " " + Integer.toOctalString(64));
        System.out.println(Integer.bitCount(255) + " " + Integer.bitCount(-1) + " " + Integer.reverse(1) + " " + Integer.reverseBytes(0x01020304) + " " + Integer.highestOneBit(100) + " " + Integer.lowestOneBit(96));
        System.out.println(Integer.numberOfLeadingZeros(1) + " " + Integer.numberOfLeadingZeros(0) + " " + Integer.numberOfTrailingZeros(8) + " " + Integer.numberOfTrailingZeros(0));
        System.out.println(Integer.rotateLeft(1, 31) + " " + Integer.rotateRight(1, 1) + " " + Integer.signum(-9) + " " + Integer.signum(0) + " " + Integer.compare(3, 9) + " " + Integer.compareUnsigned(-1, 1));
        System.out.println(Integer.divideUnsigned(-1, 2) + " " + Integer.remainderUnsigned(-1, 10) + " " + Integer.toUnsignedString(-1) + " " + Integer.toUnsignedLong(-1) + " " + Integer.toUnsignedString(-1, 16));
        System.out.println(Integer.MAX_VALUE + " " + Integer.MIN_VALUE + " " + Integer.SIZE + " " + Integer.BYTES + " " + Integer.sum(Integer.MAX_VALUE, 1) + " " + Integer.max(3, 9) + " " + Integer.min(3, 9) + " " + Integer.hashCode(77));
        System.out.println(Integer.valueOf(12) + " " + Integer.valueOf("34") + " " + Integer.decode("0x1F") + " " + Integer.decode("-010") + " " + Integer.valueOf("7f", 16));
    }
}
