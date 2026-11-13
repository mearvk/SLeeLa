class Res implements AutoCloseable {
    String n;
    boolean failOnClose;
    Res(String n, boolean failOnClose) { this.n = n; this.failOnClose = failOnClose; System.out.println("open " + n); }
    public void close() {
        System.out.println("close " + n);
        if (failOnClose) { throw new IllegalStateException("close failed " + n); }
    }
    void use(boolean fail) { if (fail) { throw new IllegalStateException("use failed " + n); } }
}
public class Resources {
    static int early() {
        try (Res d = new Res("d", false)) { return 7; }
    }
    public static void main(String[] args) {
        try (Res a = new Res("a", false); Res b = new Res("b", false)) {
            a.use(false);
            b.use(false);
            System.out.println("body");
        }
        try (Res c = new Res("c", false)) {
            c.use(true);
        } catch (IllegalStateException e) {
            System.out.println("caught " + e.getMessage());
        } finally {
            System.out.println("fin");
        }
        System.out.println(early());
        try (Res e = new Res("e", true)) {
            e.use(true);
        } catch (IllegalStateException ex) {
            System.out.println(ex.getMessage() + " suppressed=" + ex.getSuppressed().length);
        }
        try (Res f = new Res("f", true)) {
            System.out.println("ok body");
        } catch (IllegalStateException ex) {
            System.out.println(ex.getMessage() + " suppressed=" + ex.getSuppressed().length);
        }
        for (int i = 0; i < 2; i++) {
            try (Res g = new Res("g" + i, false)) {
                if (i == 0) { continue; }
                System.out.println("second");
            }
        }
    }
}
