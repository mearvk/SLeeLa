import java.util.UUID;

/** Java side of the UUIDCore differential test (java.util.UUID). */
public class UUIDCore {
    static void show(UUID u) {
        System.out.println(u + " v" + u.version() + " var" + u.variant() + " " + u.getMostSignificantBits()
            + " " + u.getLeastSignificantBits() + " h" + u.hashCode());
    }
    static void parse(String s) {
        try { show(UUID.fromString(s)); } catch (Exception e) { System.out.println(s + " !! " + e); }
    }
    static byte[] bytes(int n) {
        byte[] b = new byte[n];
        for (int i = 0; i < n; i++) { b[i] = (byte) (i * 7 + 3); }
        return b;
    }
    static byte[] text(String s) {
        byte[] b = new byte[s.length()];
        for (int i = 0; i < b.length; i++) { b[i] = (byte) s.charAt(i); }
        return b;
    }
    public static void main(String[] args) {
        show(new UUID(0, 0));
        show(new UUID(-1, -1));
        show(new UUID(1314564453825188563L, -6605018797301088256L));
        show(new UUID(-9223372036854775808L, 9223372036854775807L));
        show(new UUID(9223372036854775807L, -9223372036854775808L));
        show(new UUID(1234605616436508552L, -7373874951294615808L));
        show(new UUID(4295098371L, -9222246136947933179L));
        show(new UUID(1311768467463740911L, -4549198573597622271L));

        parse("123e4567-e89b-12d3-a456-426614174000");
        parse("00000000-0000-0000-0000-000000000000");
        parse("FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF");
        parse("1-2-3-4-5");
        parse("7fffffffffffffff-1-2-3-4");
        parse("1-2-3-4-+5");
        parse("");
        parse("abc");
        parse("1-2-3-4");
        parse("1-2-3-4-5-6");
        parse("zz-1-2-3-4");
        parse("1--2-3-4");
        parse("1-2-3-4-");
        parse("fffffffffffffffff-1-2-3-4");
        parse("8000000000000000-1-2-3-4");
        parse("+-1-2-3-4");
        parse("1-2-3-4- 5");
        parse("123e4567-e89b-12d3-a456-42661417400g");
        parse("123e4567-e89b-12d3-a456-4266141740000000000000000000000000");
        parse("1-2-3-4-ffffffffffffffff");

        UUID t = UUID.fromString("c232ab00-9414-11ec-b3c8-9f6bdeced846");
        System.out.println(t.version() + " " + t.timestamp() + " " + t.clockSequence() + " " + t.node());
        UUID v4 = UUID.fromString("123e4567-e89b-42d3-a456-426614174000");
        try { v4.timestamp(); } catch (Exception e) { System.out.println(e); }
        try { v4.clockSequence(); } catch (Exception e) { System.out.println(e); }
        try { v4.node(); } catch (Exception e) { System.out.println(e); }

        UUID a = new UUID(5, 6), b = new UUID(5, 6), c = new UUID(5, 7), d = new UUID(-1, 0), e = new UUID(1, 0);
        System.out.println(a.equals(b) + " " + a.equals(c) + " " + a.equals(null) + " " + a.equals("x") + " " + a.equals(a));
        System.out.println(a.compareTo(b) + " " + a.compareTo(c) + " " + c.compareTo(a) + " " + d.compareTo(e) + " " + e.compareTo(d));
        try { a.compareTo(null); } catch (Exception x) { System.out.println(x.getClass().getName()); }

        String[] names = {"", "a", "hello", "The quick brown fox jumps over the lazy dog"};
        for (String n : names) { show(UUID.nameUUIDFromBytes(text(n))); }
        int[] sizes = {1, 54, 55, 56, 57, 63, 64, 65, 119, 120, 128, 255, 256};
        for (int n : sizes) { show(UUID.nameUUIDFromBytes(bytes(n))); }

        UUID r1 = UUID.randomUUID(), r2 = UUID.randomUUID();
        System.out.println(r1.version() + " " + r1.variant() + " " + r2.version() + " " + r2.variant()
            + " " + r1.equals(r2) + " " + r1.toString().length() + " " + UUID.fromString(r1.toString()).equals(r1));
    }
}
