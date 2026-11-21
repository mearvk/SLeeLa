// Calls into lib/java members whose declared parameter types accept the arguments: the member check
// compares parameter types, and a widening argument (int to long, int to double) still matches.
import java.util.Arrays;

public class LibraryOverloads {
    public static void main(String[] args) {
        int[] a = new int[4];
        Arrays.fill(a, 3);
        System.out.println(a[0] + a[3]);
        StringBuilder sb = new StringBuilder();
        sb.append(1).append(2L).append(2.5).append("s").append(true);
        System.out.println(sb);
        System.out.println(Math.max(3, 4L) + " " + Math.abs(-2.5) + " " + Math.max(1, 2));
    }
}
