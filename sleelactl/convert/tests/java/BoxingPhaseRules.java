public class BoxingPhaseRules {
    static String choose(Long value) { return "long-wrapper"; }
    static String choose(Object value) { return "object"; }

    static long unboxAndWiden(long value) { return value; }

    public static void main(String[] args) {
        Integer boxed = 7;
        System.out.println(choose(1));
        System.out.println(unboxAndWiden(boxed));
    }
}
