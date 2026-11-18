public class BoxingOverload {
    static String pick(int value) { return "primitive"; }
    static String pick(Integer value) { return "boxed"; }
    static String boxedOnly(Integer value) { return "boxed-only"; }
    static String unboxedOnly(int value) { return "unboxed-only"; }

    public static void main(String[] args) {
        Integer boxed = 9;
        System.out.println(pick(7));
        System.out.println(boxedOnly(7));
        System.out.println(pick(boxed));
        System.out.println(unboxedOnly(boxed));
    }
}
