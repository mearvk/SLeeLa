// Field defaults and initialization order (JLS 4.12.5, 8.6, 12.5).
public class Init {
    static int steps;
    static int log(String s) { System.out.println(s); return ++steps; }

    static int si; static long sl; static double sd; static boolean sb; static char sc; static String ss; static int[] sa;
    int ii; long il; double id; boolean ib; char ic; String is; byte iby; short ish; float ifl; int[] ia;

    static class A {
        int a = log("A.field");
        int b = a + 1;
        { log("A.block"); }
        A() { this(7); log("A()"); }
        A(int x) { log("A(int) " + x + " b=" + b); }
        void show() { log("A.show"); }
    }
    static class B extends A {
        int c = log("B.field");
        B() { super(); log("B.ctor"); }
        void show() { log("B.show c=" + c); }
    }
    static abstract class Base { Base() { init(); } abstract void init(); }
    static class Derived extends Base {
        int v = 5;
        String name = "d";
        void init() { System.out.println("init v=" + v + " name=" + name); }
    }
    static class Plain extends A { int p = log("Plain.p"); }
    static class MyEx extends Exception {
        int code;
        MyEx(String m, int c) { super(m); code = c; }
    }
    static void fail(int n) throws MyEx { if (n > 1) throw new MyEx("big " + n, n); }

    public static void main(String[] args) {
        System.out.println(si + " " + sl + " " + sd + " " + sb + " " + (int) sc + " " + ss + " " + sa);
        Init t = new Init();
        System.out.println(t.ii + " " + t.il + " " + t.id + " " + t.ib + " " + (int) t.ic + " " + t.is + " " + t.iby + " " + t.ish + " " + t.ifl + " " + t.ia);
        si += 5; t.ii++; t.il += 2; System.out.println(si + " " + t.ii + " " + t.il);
        new B();
        new Plain();
        new Derived();
        try { fail(1); fail(5); } catch (MyEx e) { System.out.println(e.getMessage() + e.code); }
    }
}
