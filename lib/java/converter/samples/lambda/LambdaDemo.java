// Sample for lib/java/converter: lambdas and method references lowered to (id, env).
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.IntBinaryOperator;
import java.util.function.Predicate;
import java.util.function.Supplier;
import java.util.function.Consumer;
import java.util.Comparator;

public class LambdaDemo {
    private int base = 100;
    private static int counter = 0;

    static int twice(int x) { return 2 * x; }

    int plusBase(int x) { return x + base; }

    static int apply(IntBinaryOperator op, int a, int b) { return op.applyAsInt(a, b); }

    Function<Integer, Integer> adder(int k) {
        return x -> x + k + base;
    }

    static Supplier<String> greeter(String who) {
        return () -> "hello " + who;
    }

    void run() {
        int k = 3;
        Function<Integer, Integer> times = x -> x * k;
        System.out.println(times.apply(14));
        BiFunction<Integer, Integer, Integer> add = (a, b) -> a + b;
        System.out.println(add.apply(20, 22));
        IntBinaryOperator mul = (int a, int b) -> a * b;
        System.out.println(mul.applyAsInt(6, 7));
        System.out.println(apply((a, b) -> a - b, 50, 8));
        Predicate<String> empty = s -> s.isEmpty();
        System.out.println(empty.test(""));
        System.out.println(empty.test("x"));
        Comparator<String> byLength = (s, t) -> s.length() - t.length();
        System.out.println(byLength.compare("abc", "de") > 0);
        Supplier<String> hi = greeter("world");
        System.out.println(hi.get());
        System.out.println(adder(5).apply(1));
        Function<Integer, Integer> viaThis = x -> plusBase(x);
        System.out.println(viaThis.apply(7));
        Consumer<String> show = System.out::println;
        show.accept("printed by a method reference");
        Function<Integer, Integer> dbl = LambdaDemo::twice;
        System.out.println(dbl.apply(21));
        Function<Integer, Integer> pb = this::plusBase;
        System.out.println(pb.apply(1));
        Function<String, Integer> len = String::length;
        System.out.println(len.apply("four"));
        Runnable tick = () -> { counter = counter + 1; };
        tick.run();
        tick.run();
        System.out.println(counter);
        Function<Integer, Function<Integer, Integer>> curried = a -> b -> a * 10 + b;
        System.out.println(curried.apply(4).apply(2));
        Function<Integer, String> block = n -> {
            int sum = 0;
            for (int i = 1; i <= n; i++) { sum += i; }
            return "sum " + sum;
        };
        System.out.println(block.apply(10));
    }

    public static void main(String[] args) {
        new LambdaDemo().run();
    }
}
