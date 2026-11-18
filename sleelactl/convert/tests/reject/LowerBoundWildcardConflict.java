// expect: generic-inference
public class LowerBoundWildcardConflict {
    static class Bag<T> { }

    static <T> T keep(Bag<? super T> sink, T value) {
        return value;
    }

    public static void main(String[] args) {
        Bag<String> sink = new Bag<String>();
        System.out.println(keep(sink, Integer.valueOf(7)));
    }
}
