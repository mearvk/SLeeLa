// expect: intersection-cast
// An intersection cast makes the lambda both a Comparator and Serializable (JLS 15.16, 15.27.3);
// a SLeeLa value cannot carry the extra interface type.
import java.io.Serializable;
import java.util.Comparator;

public class IntersectionCast {
    public static void main(String[] args) {
        Comparator<String> c = (Comparator<String> & Serializable) (a, b) -> a.length() - b.length();
        System.out.println(c.compare("a", "bb"));
    }
}
