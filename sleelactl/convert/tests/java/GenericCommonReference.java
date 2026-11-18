public class GenericCommonReference {
    static class Parent { }
    static class Child extends Parent { }

    static <T> T choose(T first, T second) {
        return first;
    }

    static int accept(Parent value) {
        return 1;
    }

    public static void main(String[] args) {
        System.out.println(accept(choose(new Child(), new Parent())));
    }
}
