import java.util.Base64;

/** Java side of the Base64Core differential test (java.util.Base64 and its nested Encoder / Decoder). */
public class Base64Core {
    static byte[] pattern(int n, int seed) {
        byte[] b = new byte[n];
        for (int i = 0; i < n; i++) { b[i] = (byte) ((i * 37 + seed) % 256); }
        return b;
    }
    static String ints(byte[] b) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < b.length; i++) { if (i > 0) { sb.append(','); } sb.append((int) b[i]); }
        return "[" + sb + "]";
    }
    static byte[] ascii(String s) {
        byte[] b = new byte[s.length()];
        for (int i = 0; i < b.length; i++) { b[i] = (byte) s.charAt(i); }
        return b;
    }
    static String vis(String s) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.length(); i++) {
            int c = s.charAt(i);
            if (c == 13) { sb.append("<CR>"); } else if (c == 10) { sb.append("<LF>"); } else { sb.append((char) c); }
        }
        return sb.toString();
    }
    static String tryDecode(Base64.Decoder d, String s) {
        try { return ints(d.decode(s)); } catch (IllegalArgumentException e) { return "IAE:" + e.getMessage(); }
    }
    public static void main(String[] args) {
        Base64.Encoder enc = Base64.getEncoder();
        for (int n = 0; n <= 7; n++) {
            System.out.println(n + " " + enc.encodeToString(pattern(n, 5)) + " " + enc.withoutPadding().encodeToString(pattern(n, 5)));
        }
        System.out.println(enc.encodeToString(ascii("Hello, World!")) + " " + enc.encodeToString(ascii("Man")) + " " + enc.encodeToString(ascii("Ma")) + " " + enc.encodeToString(ascii("M")));
        byte[] edge = new byte[] { (byte) 251, (byte) 255, (byte) 191, (byte) 254, (byte) 63, (byte) 62 };
        System.out.println(enc.encodeToString(edge) + " " + Base64.getUrlEncoder().encodeToString(edge) + " " + Base64.getUrlEncoder().withoutPadding().encodeToString(pattern(4, 250)));
        System.out.println(ints(enc.encode(ascii("abc"))) + " " + enc.encode(new byte[0]).length);
        byte[] big = pattern(100, 9);
        System.out.println(vis(Base64.getMimeEncoder().encodeToString(big)));
        System.out.println(Base64.getMimeEncoder(8, new byte[] { '|' }).encodeToString(pattern(20, 1)));
        System.out.println(vis(Base64.getMimeEncoder(10, new byte[] { '\n' }).encodeToString(pattern(15, 2))) + " " + vis(Base64.getMimeEncoder(3, new byte[] { '\n' }).encodeToString(pattern(5, 2))));
        System.out.println(Base64.getMimeEncoder(76, new byte[0]).encodeToString(pattern(60, 3)).length());
        try { Base64.getMimeEncoder(8, new byte[] { 'A' }); } catch (IllegalArgumentException e) { System.out.println(e.getMessage()); }
        try { Base64.getMimeEncoder(8, new byte[] { '=' }); } catch (IllegalArgumentException e) { System.out.println(e.getMessage()); }
        byte[] dst = new byte[12];
        System.out.println(enc.encode(ascii("Hello"), dst) + " " + ints(dst));
        try { enc.encode(ascii("Hello"), new byte[4]); } catch (IllegalArgumentException e) { System.out.println(e.getMessage()); }

        Base64.Decoder dec = Base64.getDecoder();
        String[] ok = { "", "TWFu", "TWE=", "TQ==", "SGVsbG8sIFdvcmxkIQ==", "AQID", "+/8=" };
        for (String s : ok) { System.out.println("[" + s + "] " + tryDecode(dec, s)); }
        System.out.println(tryDecode(dec, "TQ") + " " + tryDecode(dec, "TWE"));
        String[] bad = { "T", "TWFu!", "TW Fu", "=TWF", "TWFuTQ==x", "TWFuT", "TQ=", "TQ=x", "T===", "TWF=Q", "-_8=" };
        for (String s : bad) { System.out.println("[" + s + "] " + tryDecode(dec, s)); }
        Base64.Decoder url = Base64.getUrlDecoder();
        System.out.println(tryDecode(url, "-_8=") + " " + tryDecode(url, "+/8=") + " " + tryDecode(url, "-_8"));
        Base64.Decoder mime = Base64.getMimeDecoder();
        System.out.println(tryDecode(mime, "TWFu\r\nTWFu") + " " + tryDecode(mime, "T W F u") + " " + tryDecode(mime, "TW!!Fu##") + " " + tryDecode(mime, "TQ==\r\n") + " " + tryDecode(mime, "!") + " " + tryDecode(mime, "T"));
        byte[] outd = new byte[3];
        System.out.println(dec.decode("TWFu".getBytes(), outd) + " " + ints(outd));
        try { dec.decode("TWFuTWFu".getBytes(), new byte[3]); } catch (IllegalArgumentException e) { System.out.println(e.getMessage()); }
        int mismatches = 0;
        for (int n = 0; n <= 40; n++) {
            byte[] src = pattern(n, n * 3);
            if (!java.util.Arrays.equals(src, dec.decode(enc.encodeToString(src)))) { mismatches++; }
            if (!java.util.Arrays.equals(src, dec.decode(enc.withoutPadding().encodeToString(src)))) { mismatches++; }
            if (!java.util.Arrays.equals(src, url.decode(Base64.getUrlEncoder().encodeToString(src)))) { mismatches++; }
            if (!java.util.Arrays.equals(src, mime.decode(Base64.getMimeEncoder().encodeToString(src)))) { mismatches++; }
        }
        System.out.println("roundtrip mismatches=" + mismatches);
        System.out.println(Base64.getMimeEncoder().encodeToString(pattern(57, 4)).length() + " " + Base64.getMimeEncoder().encodeToString(pattern(58, 4)).length() + " " + Base64.getMimeEncoder().encodeToString(pattern(114, 4)).length());
    }
}
