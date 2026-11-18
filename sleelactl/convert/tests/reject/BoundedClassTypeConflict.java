// expect: generic-type
public class BoundedClassTypeConflict {
    static class Box<T extends Number> {
        T value;
    }

    public static void main(String[] args) {
        Box<String> box = null;
        System.out.println(box == null);
    }
}
