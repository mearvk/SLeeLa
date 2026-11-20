// expect: library-stub
// The lib/java/jdk-28 StandardCharsets unit declares UTF_8 only as a placeholder
// (`static Any UTF_8 = null;`); a converted read would see null, not the charset.
import static java.nio.charset.StandardCharsets.UTF_8;

public class LibraryFieldPlaceholder {
    public static void main(String[] args) {
        System.out.println(UTF_8.name());
    }
}
