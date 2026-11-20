public class WildcardGeneric {
    static class Bag<T> { }
    static class Parent { }
    static class Child extends Parent { }

    static <T> T first(Bag<? extends T> values, T fallback) {
        return fallback;
    }

    static void acceptsAny(java.util.List<?> values) { }
    static void acceptsSuper(java.util.List<? super Child> values) { }
    static void acceptsArray(Bag<? extends Parent>[] values) { }

    public static void main(String[] args) {
        Bag<Child> bag = new Bag<Child>();
        java.util.List<Child> children = null;
        java.util.List<Parent> parents = null;
        Bag<Child>[] childBags = null;
        Child child = first(bag, new Child());
        acceptsAny(children);
        acceptsSuper(parents);
        acceptsArray(childBags);
        System.out.println(child != null);
    }
}
