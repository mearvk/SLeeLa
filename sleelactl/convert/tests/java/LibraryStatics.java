import java.util.Arrays;
import java.util.Objects;

// Java static utilities that the lib/java tier implements as instance methods
// (called as new Arrays().fill(...), the tier's documented convention).
public class LibraryStatics {
    public static void main(String[] args) {
        int[] a = new int[4];
        Arrays.fill(a, 7);
        System.out.println(a[0] + a[3]);
        String s = Objects.requireNonNull("kept", "must not be null");
        System.out.println(s);
        try {
            Objects.requireNonNull(null, "was null");
        } catch (NullPointerException e) {
            System.out.println("NPE " + e.getMessage());
        }
    }
}
