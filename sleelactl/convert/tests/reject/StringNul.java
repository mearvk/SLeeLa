// expect: string-nul
// A SLeeLa string ends at NUL, so a literal that contains one cannot keep its Java length.
public class StringNul {
    public static void main(String[] args) {
        String s = "a\0b";
        System.out.println(s.length());
    }
}
