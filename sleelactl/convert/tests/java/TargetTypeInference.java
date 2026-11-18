public class TargetTypeInference {
    static class Holder<T> { }

    static <T> T nil() {
        return null;
    }

    static <T> Holder<T> emptyHolder() {
        return null;
    }

    public static void main(String[] args) {
        String text = nil();
        Holder<String> holder = emptyHolder();
        System.out.println(text == null);
        System.out.println(holder == null);
    }
}
