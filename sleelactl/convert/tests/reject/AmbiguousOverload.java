// expect: overload-ambiguous
// Neither pick(int, long) nor pick(long, int) is more specific for pick(1, 1);
// javac reports "reference to pick is ambiguous".
public class AmbiguousOverload {
    static String pick(int a, long b) { return "int-long"; }
    static String pick(long a, int b) { return "long-int"; }
    public static void main(String[] args) {
        System.out.println(pick(1, 1));
    }
}
