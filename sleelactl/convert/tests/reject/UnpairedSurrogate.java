// expect: string-surrogate
// A Java string may hold a lone UTF-16 surrogate; a SLeeLa string is UTF-8 and cannot.
public class UnpairedSurrogate {
    public static void main(String[] args) {
        String s = "a\uD800b";
        System.out.println(s.length());
    }
}
