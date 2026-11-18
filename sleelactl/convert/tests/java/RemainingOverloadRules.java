public class RemainingOverloadRules {
    static String phase(long value) { return "primitive-widening"; }
    static String phase(Integer value) { return "boxing"; }

    static String reference(Number value) { return "number"; }
    static String reference(Object value) { return "object"; }

    static String nullable(Object value) { return "object"; }
    static String nullable(String value) { return "string"; }

    static String arity(String value) { return "fixed"; }
    static String arity(String... values) { return "varargs"; }

    static class Parent {
        String inherited(Number value) { return "parent"; }
    }
    static class Child extends Parent {
        @Override String inherited(Number value) { return "child"; }
    }

    public static void main(String[] args) {
        Integer boxed = 8;
        System.out.println(phase(1));
        System.out.println(reference(boxed));
        System.out.println(nullable(null));
        System.out.println(arity("one"));
        System.out.println(arity("one", "two"));
        Integer inheritedValue = 3;
        System.out.println(new Child().inherited(inheritedValue));
    }
}
