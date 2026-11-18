public class LowerBoundWildcardInference {
    static class Bag<T> { }

    static <T> T keep(Bag<? super T> sink, T value) {
        return value;
    }

    static <T> T fromTarget(Bag<? super T> sink) {
        return null;
    }

    public static void main(String[] args) {
        Bag<Object> sink = new Bag<Object>();
        String value = keep(sink, "lower-bound");
        String target = fromTarget(sink);
        System.out.println(value);
        System.out.println(target == null);
    }
}
