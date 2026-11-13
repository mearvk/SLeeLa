/** Java side of the IntegerParse differential test: Integer parsing and its NumberFormatException messages. */
public class IntegerParse {
    static String p(String s, int radix) {
        try { return String.valueOf(Integer.parseInt(s, radix)); } catch (NumberFormatException e) { return "NFE:" + e.getMessage(); }
    }
    static String u(String s, int radix) {
        try { return String.valueOf(Integer.parseUnsignedInt(s, radix)); } catch (NumberFormatException e) { return "NFE:" + e.getMessage(); }
    }
    static String d(String s) {
        try { return String.valueOf(Integer.decode(s)); } catch (NumberFormatException e) { return "NFE:" + e.getMessage(); }
    }
    public static void main(String[] args) {
        String[] good = { "0", "7", "-7", "+7", "2147483647", "-2147483648", "00012", "-0" };
        for (String s : good) { System.out.println(s + " -> " + p(s, 10)); }
        String[] bad = { "", "-", "+", "abc", "12x", "2147483648", "-2147483649", "99999999999999999999", " 1", "1 ", "--1" };
        for (String s : bad) { System.out.println("[" + s + "] -> " + p(s, 10)); }
        System.out.println(p("ff", 16) + " " + p("-FF", 16) + " " + p("zz", 36) + " " + p("102", 2) + " " + p("80000000", 16) + " " + p("-80000000", 16));
        System.out.println(p("1", 1) + " " + p("1", 37));
        try { Integer.parseInt(null); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }
        System.out.println(u("4294967295", 10) + " " + u("0", 10) + " " + u("+7", 10) + " " + u("ffffffff", 16));
        System.out.println(u("-5", 10) + " " + u("4294967296", 10) + " " + u("x", 10) + " " + u("", 10) + " " + u("99999999999999999999", 10));
        String[] dec = { "42", "-42", "+42", "0x1F", "-0x1f", "#FF", "010", "-010", "0", "-0", "0x", "", "-", "0xZZ", "0x-5", "2147483647", "-2147483648", "2147483648", "0xFFFFFFFFF", "08", "1_0" };
        for (String s : dec) { System.out.println("decode [" + s + "] -> " + d(s)); }
        try { Integer.decode(null); System.out.println("no throw"); } catch (NumberFormatException e) { System.out.println("NFE:" + e.getMessage()); } catch (NullPointerException e) { System.out.println("NPE"); }
        System.out.println(Integer.valueOf("12") + " " + Integer.valueOf("ff", 16) + " " + new Integer("77") + " " + new Integer(5));
        try { Integer.valueOf("q"); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }
        try { new Integer("q"); } catch (NumberFormatException e) { System.out.println(e.getMessage()); }
    }
}
