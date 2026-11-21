// expect: string-static
// String is a keyword type in SLeeLa, so its static fields cannot be read as String.X.
import java.util.Comparator;

public class StringStaticField {
    public static void main(String[] args) {
        Comparator<String> c = String.CASE_INSENSITIVE_ORDER;
        System.out.println(c.compare("a", "A"));
    }
}
