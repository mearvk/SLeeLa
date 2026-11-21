// expect: library-member
// The lib/java/jdk-28 Arrays unit implements fill(int[], int) and declares fill(long[], long) not
// at all. The member check used to compare only the argument count, so this call converted and
// failed when linked ("argument to 'fill' has type long[], expected int[]"). Parameter types are
// now compared.
import java.util.Arrays;

public class LibraryParameterTypes {
    public static void main(String[] args) {
        long[] a = new long[3];
        Arrays.fill(a, 7L);
        System.out.println(a[0]);
    }
}
