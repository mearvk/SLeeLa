interface IntOp { int apply(int a, int b); }
interface Maker { Box make(int v); }
interface Show { String show(Box b); }
class Box {
    int v;
    Box(int v) { this.v = v; }
    int plus(int d) { return v + d; }
    String label() { return "box" + v; }
}
public class Refs {
    int bias = 7;
    static int add(int a, int b) { return a + b; }
    int withBias(int a, int b) { return a + b + bias; }
    static int run(IntOp op, int x, int y) { return op.apply(x, y); }
    static Object describe(Object o) {
        if (o instanceof Box b && b.v > 1) { return "big box " + b.v; }
        if (o instanceof Box b2) { return "box " + b2.v; }
        if (!(o instanceof String s)) { return "other"; }
        return "string " + s.length();
    }
    int viaThis() { return run(this::withBias, 1, 2); }
    public static void main(String[] args) {
        System.out.println(run(Refs::add, 3, 4));
        IntOp f = Refs::add;
        System.out.println(f.apply(10, 20));
        Maker mk = Box::new;
        Box b = mk.make(9);
        System.out.println(b.v);
        Show sh = Box::label;
        System.out.println(sh.show(b));
        Refs r = new Refs();
        System.out.println(r.viaThis());
        IntOp bound = r::withBias;
        System.out.println(bound.apply(1, 1));
        System.out.println(describe(new Box(5)) + "|" + describe(new Box(1)) + "|" + describe("abc") + "|" + describe(3));
    }
}
