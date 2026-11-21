// expect: class-literal
// With only `import java.awt.*;`, List is java.awt.List. The lib/java unit chosen for the simple
// name List is java.util.List, so the binary name cannot be taken from it: the literal is refused
// instead of being written as java.util.List.
import java.awt.*;

public class ClassLiteralPackage {
    public static void main(String[] args) {
        System.out.println(List.class.getName());
    }
}
