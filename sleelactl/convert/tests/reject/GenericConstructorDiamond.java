// expect: generic-constructor-diamond
// Diamond allocation needs constructor-argument and target-type inference.
// The converter must refuse rather than erase <> and emit a raw allocation.
class GenericConstructorDiamond {
    static final class Box<T> {
        final T value;
        Box(T value) { this.value = value; }
    }
    public static void main(String[] args) {
        Box<String> box = new Box<>("diamond");
        System.out.println(box.value);
    }
}
