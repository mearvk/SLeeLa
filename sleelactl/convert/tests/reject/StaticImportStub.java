// expect: library-stub
// Objects.requireNonNull(Object) is declared in the lib/java/jdk-28 Objects unit only as a
// signature stub that returns null. The bare call through the static import was emitted unchecked.
import static java.util.Objects.requireNonNull;

public class StaticImportStub {
    public static void main(String[] args) {
        String s = requireNonNull("x");
        System.out.println(s);
    }
}
