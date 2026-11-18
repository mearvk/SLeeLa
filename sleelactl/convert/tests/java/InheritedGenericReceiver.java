public class InheritedGenericReceiver {
    static class Base<T> {
        T value;
        T get() {
            return null;
        }
    }

    static class Child extends Base<String> { }

    public static void main(String[] args) {
        Child child = null;
        String value = child.get();
        String field = child.value;
        System.out.println(value);
        System.out.println(field);
    }
}
