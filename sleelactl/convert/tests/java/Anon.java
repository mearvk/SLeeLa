interface Greeter { String greet(String n); }
interface Op { int apply(int a, int b); }
interface Task { void run(); }
abstract class Base {
    int k;
    Base(int k) { this.k = k; }
    abstract int f(int x);
    int twice(int x) { return f(f(x)); }
}
public class Anon {
    int factor = 3;
    static int counter = 0;
    int scale(int v) { return v * factor; }
    static int run(Op op, int x, int y) { return op.apply(x, y); }
    static void exec(Task t) { t.run(); }
    Greeter makeGreeter(String prefix) {
        int n = 2;
        return new Greeter() {
            public String greet(String who) { return prefix + who + n + scale(n) + factor; }
        };
    }
    Op makeOp(int bias) {
        return (a, b) -> a * factor + b + bias;
    }
    int self() {
        Op o = (a, b) -> this.scale(a) + b;
        return o.apply(4, 5);
    }
    public static void main(String[] args) {
        Anon an = new Anon();
        Greeter g = an.makeGreeter("hi ");
        System.out.println(g.greet("bob"));
        System.out.println(an.makeOp(100).apply(2, 3));
        System.out.println(an.self());
        Op add = (a, b) -> a + b;
        Op mul = (int a, int b) -> { int r = a * b; return r; };
        System.out.println(add.apply(3, 4) + " " + mul.apply(3, 4));
        int base = 10;
        System.out.println(run((a, b) -> a - b + base, 7, 2));
        exec(() -> { counter++; });
        exec(() -> counter += 10);
        System.out.println(counter);
        Base b = new Base(5) {
            int f(int x) { return x + k; }
        };
        System.out.println(b.twice(1) + " " + b.k);
        Task[] tasks = new Task[2];
        for (int i = 0; i < 2; i++) {
            final int id = i;
            tasks[i] = () -> System.out.println("task " + id);
        }
        for (Task t : tasks) { t.run(); }
    }
}
