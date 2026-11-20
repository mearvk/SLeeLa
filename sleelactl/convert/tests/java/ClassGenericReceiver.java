public class ClassGenericReceiver {
    static class Box<T> {
        T value;
        T get() {
            return value;
        }
    }

    public static void main(String[] args) {
        // A real receiver: a null one would test Java's NullPointerException text,
        // which SLeeLa does not reproduce (null dereference is fatal, RUNTIME-AUDIT.md).
        Box<String> box = new Box<String>();
        box.value = "class-generic";
        String value = box.get();
        String field = box.value;
        System.out.println(value);
        System.out.println(field);
    }
}
