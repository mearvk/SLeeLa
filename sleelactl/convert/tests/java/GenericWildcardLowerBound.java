import java.util.List;

public class GenericWildcardLowerBound {
    static <T> T select(List<? super T> destination, List<T> source, T fallback) {
        return fallback;
    }

    public static void main(String[] args) {
        List<Object> destination = null;
        List<String> source = null;
        String selected = select(destination, source, "lower-bound-ok");
        System.out.println(selected);
    }
}
