public class WildcardGeneric {
    static class Bag<T> { }
    static class Parent { }
    static class Child extends Parent { }

    static <T> T first(Bag<? extends T> values, T fallback) {
        return fallback;
    }

    static void acceptsAny(java.util.List<?> values) { }
    static void acceptsSuper(java.util.List<? super Child> values) { }

    public static void main(String[] args) {
        java.util.List<Child> children = null;
        java.util.List<Parent> parents = null;
        Child child = first(children, new Child());
        acceptsAny(children);
        acceptsSuper(parents);
        System.out.println(child != null);
    }
}
