import java.util.ArrayList;

// Annotations on locals, parameters and members, with dotted names and array values, have no
// run-time effect (JLS 9.7) and are dropped by the conversion.
public class Annotated {
    @SuppressWarnings({"rawtypes", "unchecked"})
    static int count(@java.lang.Deprecated ArrayList<String> xs, final @SuppressWarnings("unused") int extra) {
        @SuppressWarnings("unchecked")
        ArrayList<String> copy = (ArrayList<String>) xs;
        @SuppressWarnings(value = {"a", "b"}) final int n = copy.size();
        return n + extra;
    }

    @Deprecated(since = "1", forRemoval = false)
    public static void main(String[] args) {
        ArrayList<String> xs = new ArrayList<String>();
        xs.add("a");
        xs.add("b");
        System.out.println(count(xs, 10));
    }
}
