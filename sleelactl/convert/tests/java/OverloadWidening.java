public class OverloadWidening {
    static class Parent {
        String inherited(String x) { return "parent-string"; }
    }
    static class Child extends Parent {
        String inherited(Object x) { return "child-object"; }
    }
    static String pick(int x) { return "int"; }
    static String pick(long x) { return "long"; }
    static String pick(double x) { return "double"; }
    static String fixed(String x) { return "fixed"; }
    static String fixed(String... x) { return "varargs"; }
    static int count(String... x) { return x.length; }
    static int lambdas(Runnable... x) { return x.length; }
    static float asFloat(float x) { return x; }
    static double asDouble(double x) { return x; }
    public static void main(String[] args) {
        byte b = 1;
        float f = 1.5f;
        Child c = new Child();
        System.out.println(pick(b) + " " + pick(1L) + " " + fixed("x") + " " +
                           count("a", "b") + " " + asFloat(b) + " " + asDouble(f) + " " +
                           c.inherited("x") + " " + lambdas(() -> {}, () -> {}));
    }
}
