import java.util.Map;
import java.util.ArrayList;

// Class literals (JLS 15.8.2) become Class.ofLiteral(binary name, kind): one Class object per name,
// so == holds, and the name-based members answer as in Java.
public class ClassLiterals {
    interface Shape { }
    enum Color { RED }
    record Point(int x, int y) { }
    static class Box { }

    public static void main(String[] args) {
        System.out.println(String.class.getName());
        System.out.println(int.class.getName() + " " + int.class.isPrimitive());
        System.out.println(int[].class.getName() + " " + int[].class.isArray());
        System.out.println(Object[].class.getName());
        System.out.println(String[][].class.getName());
        System.out.println(Box.class.getName() + " " + Box.class.getSimpleName());
        System.out.println(Shape.class.isInterface() + " " + Color.class.isEnum() + " " + Point.class.isRecord());
        System.out.println(Map.Entry.class.getName());
        System.out.println(ArrayList.class.getName());
        System.out.println(String.class == String.class);
        Class<?> any = Box.class;
        System.out.println(any == Shape.class);
        Class<?> c = Box[].class;
        System.out.println(c.getName() + " " + c.isArray());
        System.out.println(void.class.getName());
    }
}
