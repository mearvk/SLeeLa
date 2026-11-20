// expect: generic-array
// T[] erases to Any[]; SLeeLa arrays are invariant and have no array cast
// (SLEELA.syntax 46), so a String[] cannot be passed as, or recovered from, T[].
public class GenericArrayIdentity {
    static <T> T[] sameArray(T[] value) {
        return value;
    }

    public static void main(String[] args) {
        System.out.println(sameArray(new String[] { "x" }).length);
    }
}
