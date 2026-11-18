public class ClassGenericReceiver {
    static class Box<T> {
        T value;
        T get() {
            return value;
        }
    }

    public static void main(String[] args) {
        Box<String> box = null;
        String value = box.get();
        String field = box.value;
        System.out.println(value);
        System.out.println(field);
    }
}
