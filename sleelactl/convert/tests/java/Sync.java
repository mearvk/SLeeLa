// flags: --single-thread
public class Sync {
    private volatile int hits;
    private final Object lock = new Object();
    synchronized void bump() { hits++; }
    int read() { synchronized (lock) { return hits; } }
    public static void main(String[] args) {
        Sync s = new Sync();
        for (int i = 0; i < 5; i++) { s.bump(); }
        System.out.println(s.read());
    }
}
