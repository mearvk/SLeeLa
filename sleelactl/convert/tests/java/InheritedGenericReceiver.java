public class InheritedGenericReceiver {
    static class Base<T> {
        T get() {
            return null;
        }
    }

    static class Child extends Base<String> { }

    public static void main(String[] args) {
        Child child = null;
        String value = child.get();
        System.out.println(value);
    }
}
