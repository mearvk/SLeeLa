// String literals with control characters (JLS 3.10.6, 3.10.7): backspace, form feed, octal and
// unicode escapes below U+0020 other than newline, tab and carriage return. The converter writes
// them as backslash-u escapes, which the SLeeLa lexer reads.
public class StringControl {
    public static void main(String[] args) {
        String s = "a\bb\fc\1d\u001be\u0007";
        System.out.println(s.length());
        StringBuilder codes = new StringBuilder();
        for (int i = 0; i < s.length(); i++) codes.append((int) s.charAt(i)).append(' ');
        System.out.println(codes);
        String sep = "\u001f";
        String joined = "x" + sep + "y";
        System.out.println(joined.indexOf(sep) + " " + joined.length());
        System.out.println(String.format("[%s]\u0002", "k").length());
        System.out.println("tab\tend");
    }
}
