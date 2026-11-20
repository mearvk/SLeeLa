// expect: generic-constructor-diamond
// The target (Object) is not the allocated class: inferring Box<T> from it needs
// JLS 18.5 inference across supertypes, which the converter does not implement.
class DiamondSupertypeTarget {
    static final class Box<T> {
        final T value;
        Box(T value) { this.value = value; }
    }
    public static void main(String[] args) {
        Object box = new Box<>("diamond");
        System.out.println(box != null);
    }
}
