public class Functional {
    static int hits = 0;
    static void twice(Runnable r) { r.run(); r.run(); }
    public static void main(String[] args) {
        Runnable r = () -> { hits++; };
        twice(r);
        twice(() -> hits += 5);
        System.out.println(hits);
        String tag = "x";
        Runnable p = () -> System.out.println("run " + tag + hits);
        p.run();
    }
}
