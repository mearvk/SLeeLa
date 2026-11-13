public class Records {
    record Point(int x, int y) {}
    record Named(String name, long id, boolean ok) {
        Named { if (id < 0) throw new IllegalArgumentException("neg"); }
    }
    public static void main(String[] args) {
        Point p = new Point(3, 4);
        System.out.println(p);
        System.out.println(p.x() + p.y());
        System.out.println(p.equals(new Point(3, 4)));
        System.out.println(p.equals(new Point(3, 5)));
        System.out.println(p.hashCode());
        Named n = new Named("a", 7L, true);
        System.out.println(n);
        System.out.println(n.hashCode());
        try { new Named("b", -1, false); } catch (IllegalArgumentException e) { System.out.println("caught " + e.getMessage()); }
    }
}
