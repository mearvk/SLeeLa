public class GenericConstructorExplicitArguments {
    static final class Box<T> {
        Box() {}
    }
    public static void main(String[] args) {
        Box<String> box = new Box<String>();
        System.out.println(box == null ? "null" : "explicit");
    }
}
