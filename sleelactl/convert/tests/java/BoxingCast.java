// Boxing casts (JLS 5.5): (Integer) i, (Long) l, (Double) d box to the operand's own wrapper.
// The wrapper keeps its identity for overload resolution: show((Integer) 5) calls show(Integer).
public class BoxingCast {
    static String show(int v) { return "int " + v; }
    static String show(Integer v) { return "Integer " + v; }
    static String show(long v) { return "long " + v; }
    static String show(Long v) { return "Long " + v; }

    public static void main(String[] args) {
        int i = 5;
        long l = 7L;
        double d = 2.5;
        System.out.println(show(i));
        System.out.println(show((Integer) i));
        System.out.println(show(l));
        System.out.println(show((Long) l));
        Double boxed = (Double) d;
        System.out.println(boxed + 1.0);
        Integer back = (Integer) 42;
        System.out.println(back + 1);
    }
}
