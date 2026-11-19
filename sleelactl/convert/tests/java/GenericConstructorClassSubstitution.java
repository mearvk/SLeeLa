public class GenericConstructorClassSubstitution {
    static final class Box<T> {
        private T value;
        Box(T value) { this.value = value; }
        T get() { return value; }
    }
    public static void main(String[] args) {
        Box<String> box = new Box<String>("constructor-substitution");
        System.out.println(box.get());
    }
}
