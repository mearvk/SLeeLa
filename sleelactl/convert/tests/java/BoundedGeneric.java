public class BoundedGeneric {
    static class Parent { }
    static class Child extends Parent { }

    static <T extends Parent> T keep(T value) {
        return value;
    }

    public static void main(String[] args) {
        Child child = new Child();
        Parent parent = keep(child);
        System.out.println(parent == child);
    }
}
