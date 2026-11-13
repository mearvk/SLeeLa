public class Nested {
    static int total = 0;
    int base = 100;
    static class Counter {
        int n;
        Counter(int start) { n = start; total += start; }
        int next() { n++; total++; return n; }
    }
    interface Shape { int area(); }
    static class Sq implements Shape {
        int s;
        Sq(int s) { this.s = s; }
        public int area() { return s * s; }
    }
    enum Level { LOW, HIGH }
    class Inner {
        int v;
        Inner(int v) { this.v = v; }
        int sum() { return v + base; }
        int viaOuter() { return Nested.this.base + total; }
    }
    int make(int v) { Inner i = new Inner(v); return i.sum() + i.viaOuter(); }
    static Level pick(int n) { return n > 5 ? Level.HIGH : Level.LOW; }
    public static void main(String[] args) {
        Counter c = new Counter(3);
        c.next();
        System.out.println(c.n + " " + total);
        Shape sh = new Sq(4);
        System.out.println(sh.area());
        Nested o = new Nested();
        System.out.println(o.make(5));
        System.out.println(pick(9) + " " + pick(1) + " " + Level.HIGH.ordinal());
        Nested.Counter c2 = new Nested.Counter(10);
        System.out.println(c2.next());
    }
}
