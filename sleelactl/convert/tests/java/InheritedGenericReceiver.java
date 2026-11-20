public class InheritedGenericReceiver {
    static class Base<T> {
        T value;
        T get() {
            return value;
        }
    }

    static class Child extends Base<String> { }

    public static void main(String[] args) {
        // A real receiver: a null one would test Java's NullPointerException text,
        // which SLeeLa does not reproduce (null dereference is fatal, RUNTIME-AUDIT.md).
        Child child = new Child();
        child.value = "inherited-generic";
        String value = child.get();
        String field = child.value;
        System.out.println(value);
        System.out.println(field);
    }
}
