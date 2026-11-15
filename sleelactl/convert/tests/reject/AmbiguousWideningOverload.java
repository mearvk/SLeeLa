// expect: overload-ambiguous
public class AmbiguousWideningOverload {
    static String choose(long value) { return "long"; }
    static String choose(double value) { return "double"; }
    public static void main(String[] args) {
        System.out.println(choose(1));
    }
}
