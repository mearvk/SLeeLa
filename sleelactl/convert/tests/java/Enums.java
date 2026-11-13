enum Color { RED, GREEN, BLUE }
enum Planet {
    MERCURY(3, 2), VENUS(4, 6);
    private final int mass;
    private final int radius;
    Planet(int mass, int radius) { this.mass = mass; this.radius = radius; }
    int surface() { return mass * radius; }
}
enum Op {
    ADD("+") { int apply(int a, int b) { return a + b; } },
    MUL("*") { int apply(int a, int b) { return a * b; } };
    final String sym;
    Op(String sym) { this.sym = sym; }
    abstract int apply(int a, int b);
}
public class Enums {
    static String describe(Color c) {
        switch (c) {
            case RED: return "r";
            case GREEN: return "g";
            default: return "other";
        }
    }
    static int code(Color c) {
        return switch (c) { case RED -> 1; case GREEN -> 2; case BLUE -> 3; };
    }
    public static void main(String[] args) {
        Color c = Color.GREEN;
        System.out.println(c + " " + c.name() + " " + c.ordinal());
        System.out.println(describe(Color.RED) + describe(Color.BLUE) + code(Color.BLUE));
        for (Color x : Color.values()) { System.out.println(x.ordinal() + ":" + x); }
        System.out.println(Color.valueOf("BLUE") == Color.BLUE);
        System.out.println(Color.RED.compareTo(Color.BLUE));
        System.out.println(Planet.VENUS.surface() + " " + Planet.MERCURY.surface());
        System.out.println(Op.ADD.apply(3, 4) + " " + Op.MUL.apply(3, 4) + " " + Op.MUL.sym);
        try { Color.valueOf("PINK"); } catch (IllegalArgumentException e) { System.out.println("bad"); }
        Color d = Color.RED;
        System.out.println(c == d);
        System.out.println(c != d);
    }
}
