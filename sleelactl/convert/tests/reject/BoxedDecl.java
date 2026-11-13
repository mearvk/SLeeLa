// expect: boxed
// A boxed declared type would name the lib/java class Integer, to which an int is not assignable:
// the converted program would not compile, so the converter refuses it.
public class BoxedDecl {
    public static void main(String[] args) {
        Integer i = 42;
        System.out.println(i + 1);
    }
}
