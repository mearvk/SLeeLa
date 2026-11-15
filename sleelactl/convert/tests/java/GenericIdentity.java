public class GenericIdentity {
    static <T> T identity(T value) {
        return value;
    }

    static <T> T[] sameArray(T[] value) {
        return value;
    }

    public static void main(String[] args) {
        System.out.println(identity("generic"));
        System.out.println(identity(7));
        System.out.println(sameArray(new String[] { "x" }).length);
    }
}
