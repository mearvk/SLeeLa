// choose(long) and choose(double) both accept an int by widening. This is NOT
// ambiguous: long <: double in primitive subtyping (JLS 4.10.1), so choose(long)
// is more specific (JLS 15.12.2.5) and javac prints "long".
public class WideningMostSpecific {
    static String choose(long value) { return "long"; }
    static String choose(double value) { return "double"; }
    public static void main(String[] args) {
        System.out.println(choose(1));
    }
}
