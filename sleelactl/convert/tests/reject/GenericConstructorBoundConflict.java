// expect: generic-type
// Explicit constructor type arguments must satisfy the class's declared upper bound.
class GenericConstructorBoundConflict {
    static final class Box<T extends Number> {
        Box() {}
    }
    public static void main(String[] args) {
        Box<String> box = new Box<String>();
    }
}
