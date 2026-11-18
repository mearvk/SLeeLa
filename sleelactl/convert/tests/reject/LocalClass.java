// expect: local-type
// Local classes need a lexical symbol table and capture environment distinct from nested member types.
public class LocalClass {
    public static void main(String[] args) {
        class Local { int value() { return 7; } }
        System.out.println(new Local().value());
    }
}
