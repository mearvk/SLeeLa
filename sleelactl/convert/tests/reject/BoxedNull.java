// expect: boxed-null
// A wrapper reference can be null in Java, but primitive SLeeLa storage cannot represent it.
public class BoxedNull {
    public static void main(String[] args) {
        Integer value = null;
        System.out.println(value);
    }
}
