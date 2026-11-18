public class NestedGenericInference {
    static class Holder<T> { }

    static <T> T choose(Holder<T> holder, T fallback) {
        return fallback;
    }

    public static void main(String[] args) {
        Holder<String> holder = null;
        String value = choose(holder, "nested");
        System.out.println(value);
    }
}
