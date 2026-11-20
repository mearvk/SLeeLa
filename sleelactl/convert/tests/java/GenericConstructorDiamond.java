// A diamond allocation takes its class type arguments from a target type of the
// same class (JLS 15.9.3); the inferred Box<String> keeps String for box.value.
public class GenericConstructorDiamond {
    static final class Box<T> {
        final T value;
        Box(T value) { this.value = value; }
        T get() { return value; }
    }
    public static void main(String[] args) {
        Box<String> box = new Box<>("diamond");
        System.out.println(box.value);
        String length = box.get() + " " + box.get().length();
        System.out.println(length);
        Box<String> again;
        again = new Box<>("assigned");
        System.out.println(again.get());
    }
}
