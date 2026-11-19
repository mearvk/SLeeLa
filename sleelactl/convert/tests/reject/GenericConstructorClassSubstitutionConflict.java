// expect: constructor-resolution
// Class type arguments must be substituted before constructor applicability is decided.
class GenericConstructorClassSubstitutionConflict {
    static final class Box<T> {
        Box(T value) {}
    }
    public static void main(String[] args) {
        Box<String> box = new Box<String>(7);
    }
}
