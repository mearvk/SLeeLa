public class BoundedClassType {
    static class Box<T extends Number> {
        T value;
    }

    public static void main(String[] args) {
        Box<Integer> box = new Box<Integer>();
        System.out.println(box != null);
    }
}
