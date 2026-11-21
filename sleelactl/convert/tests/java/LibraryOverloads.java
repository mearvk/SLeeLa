// Calls into lib/java members whose declared parameter types accept the arguments: the member check
// compares parameter types, and a widening argument (int to long, int to double) still matches. A
// package-qualified parameter type in a unit (java.lang.CharSequence) erases to Any and takes a String.
import java.util.Arrays;
import java.util.StringJoiner;

public class LibraryOverloads {
    public static void main(String[] args) {
        int[] a = new int[4];
        Arrays.fill(a, 3);
        System.out.println(a[0] + a[3]);
        StringBuilder sb = new StringBuilder();
        sb.append(1).append(2L).append(2.5).append("s").append(true);
        System.out.println(sb);
        StringJoiner j = new StringJoiner(", ");
        j.add("x");
        j.add("y");
        System.out.println(j);
        System.out.println(Math.max(3, 4L) + " " + Math.abs(-2.5) + " " + Math.max(1, 2));
    }
}
