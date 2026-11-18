// expect: boxed
// Boxed wrapper arrays need reference-array identity and null-element semantics.
public class BoxedDecl {
    public static void main(String[] args) {
        Integer[] values = new Integer[2];
        System.out.println(values.length);
    }
}
