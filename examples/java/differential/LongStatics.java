/** Java side of the LongStatics differential test; prints one result per line. */
public class LongStatics {
    static String msg(Runnable r) {
        try { r.run(); return "ok"; } catch (NumberFormatException e) { return "NFE"; } catch (ArithmeticException e) { return "AE"; }
    }
    public static void main(String[] args) {
        System.out.println(Long.parseLong("42") + " " + Long.parseLong("-9223372036854775808") + " " + Long.parseLong("9223372036854775807") + " " + Long.parseLong("-ff", 16) + " " + Long.parseLong("+1010", 2) + " " + Long.parseUnsignedLong("18446744073709551615") + " " + Long.parseUnsignedLong("ff", 16));
        System.out.println(Long.toString(255L) + " " + Long.toString(-255L, 16) + " " + Long.toString(Long.MIN_VALUE) + " " + Long.toString(Long.MIN_VALUE, 2) + " " + Long.toBinaryString(10L) + " " + Long.toBinaryString(-1L) + " " + Long.toHexString(-255L) + " " + Long.toOctalString(-1L));
        System.out.println(Long.toUnsignedString(-1L) + " " + Long.toUnsignedString(-1L, 36) + " " + Long.toUnsignedString(5L) + " " + Long.toHexString(Long.MAX_VALUE));
        System.out.println(Long.bitCount(255L) + " " + Long.bitCount(-1L) + " " + Long.reverse(1L) + " " + Long.reverseBytes(0x0102030405060708L) + " " + Long.highestOneBit(100L) + " " + Long.lowestOneBit(96L) + " " + Long.highestOneBit(-1L) + " " + Long.lowestOneBit(Long.MIN_VALUE));
        System.out.println(Long.numberOfLeadingZeros(1L) + " " + Long.numberOfLeadingZeros(0L) + " " + Long.numberOfLeadingZeros(-1L) + " " + Long.numberOfTrailingZeros(8L) + " " + Long.numberOfTrailingZeros(0L));
        System.out.println(Long.rotateLeft(1L, 63) + " " + Long.rotateRight(1L, 1) + " " + Long.rotateLeft(0x8000000000000001L, 1) + " " + Long.rotateRight(6L, 65) + " " + Long.rotateLeft(5L, -1));
        System.out.println(Long.signum(-9L) + " " + Long.signum(0L) + " " + Long.compare(3L, 9L) + " " + Long.compare(9L, 3L) + " " + Long.compareUnsigned(-1L, 1L) + " " + Long.compareUnsigned(1L, -1L) + " " + Long.compareUnsigned(7L, 7L));
        System.out.println(Long.divideUnsigned(-1L, 2L) + " " + Long.remainderUnsigned(-1L, 10L) + " " + Long.divideUnsigned(100L, 7L) + " " + Long.remainderUnsigned(100L, 7L) + " " + Long.divideUnsigned(-1L, -1L));
        System.out.println(Long.MAX_VALUE + " " + Long.MIN_VALUE + " " + Long.SIZE + " " + Long.BYTES + " " + Long.sum(Long.MAX_VALUE, 1L) + " " + Long.max(3L, 9L) + " " + Long.min(3L, 9L) + " " + Long.hashCode(77L) + " " + Long.hashCode(-1L) + " " + Long.hashCode(Long.MAX_VALUE) + " " + Long.hashCode(1L << 32));
        System.out.println(Long.valueOf(12L) + " " + Long.valueOf("34") + " " + Long.decode("0x1F") + " " + Long.decode("-010") + " " + Long.valueOf("7f", 16) + " " + Long.decode("#FF") + " " + Long.decode("-0x8000000000000000"));
        System.out.println(Long.compress(0xF0F0L, 0xFF00L) + " " + Long.expand(0xABL, 0xF0F0L) + " " + Long.compress(-1L, Long.MIN_VALUE) + " " + Long.expand(1L, Long.MIN_VALUE));
        Long a = Long.valueOf(300L);
        System.out.println(a.byteValue() + " " + a.shortValue() + " " + a.intValue() + " " + Long.valueOf(-5000000000L).intValue() + " " + a.longValue() + " " + (a.doubleValue() == 300.0) + " " + a.hashCode() + " " + a.equals(Long.valueOf(300L)) + " " + a.equals(Long.valueOf(301L)) + " " + a.compareTo(Long.valueOf(1L)) + " " + a.toString());
        System.out.println(msg(() -> Long.parseLong("abc")) + " | " + msg(() -> Long.parseLong("")) + " | " + msg(() -> Long.parseLong("9223372036854775808")) + " | " + msg(() -> Long.parseLong("zz", 16)) + " | " + msg(() -> Long.parseLong("-")) + " | " + msg(() -> Long.parseLong("12", 1)) + " | " + msg(() -> Long.parseLong("12", 37)) + " | " + msg(() -> Long.parseUnsignedLong("-5")) + " | " + msg(() -> Long.parseUnsignedLong("18446744073709551616")) + " | " + msg(() -> Long.divideUnsigned(1L, 0L)) + " | " + msg(() -> Long.parseLong(null)) + " | " + msg(() -> Long.decode("")));
    }
}
