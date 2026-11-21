// A boxed primitive held as Object (JLS 5.1.7) shows its Java text: the VM stores it as the primitive,
// and JavaFormat.objectText recognises the kind at run time (3.0 stays 3.0, true stays true).
public class BoxedObjectText {
    static Object pick(int k) {
        if (k == 0) return 42;
        if (k == 1) return 7L;
        if (k == 2) return 3.0;
        if (k == 3) return true;
        if (k == 4) return "text";
        return null;
    }

    public static void main(String[] args) {
        Object o = 42;
        System.out.println(o);
        Object d = 2.5;
        System.out.println(d);
        for (int k = 0; k < 6; k++) System.out.println(pick(k));
        Object big = 9000000000L;
        System.out.println("big=" + big);
        Object neg = -0.0;
        System.out.println(neg);
    }
}
