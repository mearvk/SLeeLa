// Single static imports of lib/java/jdk-28 methods (JLS 7.5.3). The tier implements
// Objects.requireNonNull(Object, String) and Objects.isNull as instance methods, so the
// bare call is written on `new Objects()`, exactly as the qualified call would be.
import static java.util.Objects.requireNonNull;
import static java.util.Objects.isNull;
import static java.lang.Math.max;

public class StaticImportLibrary {
    public static void main(String[] args) {
        String s = requireNonNull("kept", "message");
        System.out.println(s);
        System.out.println(isNull(null));
        System.out.println(max(3, 9));
    }
}
