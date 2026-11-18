// expect: anonymous-nested
// Nested generated classes need capture propagation through each enclosing lambda.
public class NestedLambda {
    public static void main(String[] args) {
        int base = 4;
        Runnable outer = () -> {
            Runnable inner = () -> System.out.println(base);
            inner.run();
        };
        outer.run();
    }
}
