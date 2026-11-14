package classlit;

import java.util.Map;
import java.util.List;

/**
 * Class literals (lib/java/converter rule 8e): the names, kinds and identity of
 * T.class for own and JDK types, primitives, void and arrays.
 */
public class ClassLitDemo {
    interface Shape { }
    enum Color { RED, GREEN }
    record Point(int x, int y) { }
    @interface Marker { }
    static class Inner {
        static class Deeper { }
    }

    static final Class<?> FIELD = List.class;

    static void show(Class<?> c) {
        System.out.println(c.getName() + " | " + c.getSimpleName() + " | " + c.getCanonicalName()
            + " | " + c.getPackageName() + " | " + c.getTypeName());
    }

    static void kinds(Class<?> c) {
        System.out.println(c + " interface=" + c.isInterface() + " array=" + c.isArray()
            + " primitive=" + c.isPrimitive() + " enum=" + c.isEnum() + " record=" + c.isRecord()
            + " annotation=" + c.isAnnotation() + " " + c.descriptorString());
    }

    public static void main(String[] args) {
        show(ClassLitDemo.class);
        show(Inner.class);
        show(Inner.Deeper.class);
        show(Map.Entry.class);
        show(Thread.State.class);
        show(String.class);
        show(java.util.ArrayList.class);
        show(int.class);
        show(void.class);
        show(int[].class);
        show(String[][].class);
        show(Inner[].class);
        kinds(Shape.class);
        kinds(Color.class);
        kinds(Point.class);
        kinds(Marker.class);
        kinds(ClassLitDemo.class);
        kinds(List.class);
        kinds(double.class);
        kinds(long[].class);
        System.out.println((String.class == String.class) + " " + (FIELD == List.class) + " " + (int.class == Integer.class));
        System.out.println(int[][].class.componentType().getName() + " " + String[].class.getComponentType().getName()
            + " " + (String.class.componentType() == null));
        System.out.println(ClassLitDemo.class.desiredAssertionStatus());
    }
}
