// An inner (non-static) class uses the type variable E of its enclosing generic
// class (JLS 6.3), as ArrayList.Itr.next() does. It was refused as [library-type].
public class InnerOuterTypeVar<E> {
    private Object[] items = new Object[4];
    private int size;

    void add(E e) { items[size++] = e; }

    class Itr {
        int cursor;
        boolean hasNext() { return cursor < size; }
        @SuppressWarnings("unchecked")
        E next() { return (E) items[cursor++]; }
    }

    Itr iterator() { return new Itr(); }

    public static void main(String[] args) {
        InnerOuterTypeVar<String> v = new InnerOuterTypeVar<String>();
        v.add("first");
        v.add("second");
        InnerOuterTypeVar<String>.Itr it = v.iterator();
        while (it.hasNext()) System.out.println(it.next());
    }
}
