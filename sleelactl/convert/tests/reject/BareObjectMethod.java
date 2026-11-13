// expect: object-method
// An unqualified call of a java.lang.Object method inside a class that does not declare it
// (as java.text.Format calls getClass()) must be refused like a.getClass(), not written.
public class BareObjectMethod {
    static class Shape {
        int id() { return hashCode(); }
    }
    public static void main(String[] args) {
        System.out.println(new Shape().id() == 0);
    }
}
