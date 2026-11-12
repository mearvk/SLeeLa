/** Java side of the SmallWrappers differential test: java.lang.Short, java.lang.Byte, java.lang.Boolean. */
public class SmallWrappers {
    static String ps(String s, int radix) {
        try { return String.valueOf(Short.parseShort(s, radix)); } catch (NumberFormatException e) { return "NFE:" + e.getMessage(); }
    }
    static String pb(String s, int radix) {
        try { return String.valueOf(Byte.parseByte(s, radix)); } catch (NumberFormatException e) { return "NFE:" + e.getMessage(); }
    }
    static String ds(String s) {
        try { return String.valueOf(Short.decode(s)); } catch (NumberFormatException e) { return "NFE:" + e.getMessage(); }
    }
    static String db(String s) {
        try { return String.valueOf(Byte.decode(s)); } catch (NumberFormatException e) { return "NFE:" + e.getMessage(); }
    }
    public static void main(String[] args) {
        System.out.println(Short.MIN_VALUE + " " + Short.MAX_VALUE + " " + Short.SIZE + " " + Short.BYTES + " " + Byte.MIN_VALUE + " " + Byte.MAX_VALUE + " " + Byte.SIZE + " " + Byte.BYTES);
        String[] sv = { "0", "123", "-123", "32767", "-32768", "32768", "-32769", "x", "", "+9" };
        for (String s : sv) { System.out.println("short [" + s + "] " + ps(s, 10) + " | " + ps(s, 16)); }
        String[] bv = { "0", "12", "-128", "127", "128", "-129", "ff", "7f", "-80", "q" };
        for (String s : bv) { System.out.println("byte [" + s + "] " + pb(s, 10) + " | " + pb(s, 16)); }
        String[] dv = { "0x7FFF", "-0x8000", "0x8000", "010", "#7F", "99999", "abc" };
        for (String s : dv) { System.out.println("decode [" + s + "] " + ds(s) + " | " + db(s)); }
        Short a = Short.valueOf((short) 1000);
        Short b = Short.valueOf("1000");
        System.out.println(a + " " + b + " " + a.equals(b) + " " + a.hashCode() + " " + a.compareTo(Short.valueOf((short) 7)) + " " + a.byteValue() + " " + a.shortValue() + " " + a.intValue() + " " + a.longValue() + " " + (a.doubleValue() == 1000.0) + " " + (a.floatValue() == 1000.0));
        Short neg = Short.valueOf((short) -2);
        System.out.println(neg + " " + neg.byteValue() + " " + Short.toUnsignedInt((short) -2) + " " + Short.toUnsignedLong((short) -1) + " " + Short.compareUnsigned((short) -1, (short) 1) + " " + Short.compare((short) 5, (short) 9) + " " + Short.reverseBytes((short) 0x0102) + " " + Short.reverseBytes((short) -2) + " " + Short.toString((short) -77) + " " + Short.hashCode((short) 12));
        Byte c = Byte.valueOf((byte) -3);
        System.out.println(c + " " + c.shortValue() + " " + c.intValue() + " " + (c.doubleValue() == -3.0) + " " + c.hashCode() + " " + c.equals(Byte.valueOf((byte) -3)) + " " + c.equals(Short.valueOf((short) -3)) + " " + c.compareTo(Byte.valueOf((byte) 10)) + " " + Byte.toUnsignedInt((byte) -3) + " " + Byte.toUnsignedLong((byte) -1) + " " + Byte.compareUnsigned((byte) -1, (byte) 1) + " " + Byte.compare((byte) 100, (byte) -100) + " " + Byte.toString((byte) 9) + " " + Byte.hashCode((byte) -9));
        System.out.println(new Short((short) 5) + " " + new Short("-6") + " " + new Byte((byte) 7) + " " + new Byte("-8"));
        try { new Short("40000"); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }
        try { new Byte("200"); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }

        System.out.println(Boolean.parseBoolean("true") + " " + Boolean.parseBoolean("TRUE") + " " + Boolean.parseBoolean("TrUe") + " " + Boolean.parseBoolean("yes") + " " + Boolean.parseBoolean("") + " " + Boolean.parseBoolean(null) + " " + Boolean.parseBoolean("truee") + " " + Boolean.parseBoolean(" true"));
        System.out.println(Boolean.valueOf(true) + " " + Boolean.valueOf("tRuE") + " " + Boolean.valueOf(false) + " " + (Boolean.valueOf(true) == Boolean.TRUE) + " " + (Boolean.valueOf("x") == Boolean.FALSE) + " " + Boolean.TRUE + " " + Boolean.FALSE);
        Boolean t = Boolean.TRUE;
        Boolean f = Boolean.valueOf(false);
        System.out.println(t.booleanValue() + " " + t.hashCode() + " " + f.hashCode() + " " + t.equals(Boolean.valueOf("true")) + " " + t.equals(f) + " " + t.equals("true") + " " + t.compareTo(f) + " " + f.compareTo(t) + " " + t.compareTo(Boolean.TRUE) + " " + t + " " + f);
        System.out.println(Boolean.compare(true, false) + " " + Boolean.compare(false, true) + " " + Boolean.compare(true, true) + " " + Boolean.hashCode(true) + " " + Boolean.hashCode(false) + " " + Boolean.toString(true) + " " + Boolean.toString(false));
        System.out.println(Boolean.logicalAnd(true, false) + " " + Boolean.logicalOr(true, false) + " " + Boolean.logicalXor(true, true) + " " + Boolean.logicalXor(true, false) + " " + new Boolean("TRUE") + " " + new Boolean(false) + " " + Boolean.getBoolean("no.such.property"));
    }
}
