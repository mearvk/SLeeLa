// expect: boxed-null
// Null arguments cannot be passed to wrapper parameters represented by primitive slots.
public class BoxedNullParameter {
    static void accept(Integer value) { System.out.println(value); }
    public static void main(String[] args) { accept(null); }
}
