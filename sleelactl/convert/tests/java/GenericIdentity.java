public class GenericIdentity {
    static <T> T identity(T value) {
        return value;
    }

    // T... is packed as the erased Any[] (see reject/GenericArrayIdentity for T[]).
    static <T> int count(T... values) {
        return values.length;
    }

    public static void main(String[] args) {
        System.out.println(identity("generic"));
        System.out.println(identity(7));
        System.out.println(count("x", "y", "z"));
    }
}
