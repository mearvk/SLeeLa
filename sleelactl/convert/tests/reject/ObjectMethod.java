// expect: object-method
public class ObjectMethod {
    static class A { }
    public static void main(String[] args) {
        A a = new A();
        System.out.println(a.equals(a));
    }
}
