public class BoundedGenericInference {
    static <T extends Number> T identity(T value) {
        return value;
    }

    public static void main(String[] args) {
        System.out.println(identity(17));
    }
}
