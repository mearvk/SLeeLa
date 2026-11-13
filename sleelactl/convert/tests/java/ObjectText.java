// Java string conversion of objects (JLS 5.1.11): library objects, interfaces, Object, null,
// user exceptions and erased generics show their toString(), not a VM handle.
import java.util.ArrayList;

public class ObjectText {
    interface Named { String name(); }
    static class N implements Named {
        public String name() { return "n"; }
        public String toString() { return "N!"; }
    }
    static class MyEx extends RuntimeException { MyEx(String m) { super(m); } }
    static class Box<T> {
        T v;
        Box(T v) { this.v = v; }
        String show() { return "box " + v; }
    }
    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder("ab");
        sb.append(12).append('c');
        System.out.println(sb);
        System.out.println("sb=" + sb);
        Exception e = new RuntimeException("rt");
        System.out.println(e);
        System.out.println("e=" + e);
        Object o = sb;
        System.out.println(o);
        ArrayList<Integer> l = new ArrayList<>();
        l.add(1); l.add(2);
        System.out.println(l);
        System.out.println("l=" + l);
        Named n = new N();
        System.out.println(n);
        System.out.println("n=" + n);
        System.out.println(new MyEx("mine").getMessage());
        System.out.println(new Box<String>("s").show());
        Object nul = null;
        System.out.println("z" + nul);
        System.out.println(nul);
    }
}
