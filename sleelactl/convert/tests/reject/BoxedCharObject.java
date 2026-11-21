// expect: boxed-reference
// A char boxed into Object is stored as an int by the VM, so println(o) would print 99, not c.
public class BoxedCharObject {
    public static void main(String[] args) {
        Object o = 'c';
        System.out.println(o);
    }
}
