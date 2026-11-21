// Type-pattern switches (JLS 14.11, 14.30): statement and expression forms, case null,
// case null + default, unnamed bindings, block arms with yield/return/break, constant labels.
public class PatternSwitch {
    interface Shape { }
    static class Circle implements Shape { int r; Circle(int r) { this.r = r; } }
    static class Square implements Shape { int s; Square(int s) { this.s = s; } }
    static class Tri implements Shape { }

    static int area(Shape sh) {
        return switch (sh) {
            case Circle c -> 3 * c.r * c.r;
            case Square q -> {
                int a = q.s * q.s;
                yield a;
            }
            default -> -1;
        };
    }

    static String kind(Object o) {
        switch (o) {
            case null -> { return "null"; }
            case String s -> { return "string " + s.length(); }
            case Circle _ -> { return "a circle"; }
            default -> { return "other"; }
        }
    }

    static String label(String o) {
        String r = switch (o) {
            case "hi" -> "greeting";
            case null -> "none";
            case String s -> "text:" + s;
        };
        return r;
    }

    static String describe(Object o) {
        return switch (o) {
            case Square q -> "square " + q.s;
            case null, default -> "fallback";
        };
    }

    public static void main(String[] args) {
        Shape[] shapes = { new Circle(2), new Square(3), new Tri() };
        int total = 0;
        for (Shape sh : shapes) {
            switch (sh) {
                case Circle c -> total += c.r;
                case Square q -> {
                    if (q.s > 2) break;
                    total += 100;
                }
                default -> total += 1000;
            }
            System.out.println(area(sh));
        }
        System.out.println(total);
        System.out.println(kind(null));
        System.out.println(kind("abc"));
        System.out.println(kind(new Circle(1)));
        System.out.println(kind(new Tri()));
        System.out.println(label("hi"));
        System.out.println(label("yo"));
        System.out.println(label(null));
        System.out.println(describe(new Square(4)));
        System.out.println(describe(null));
        System.out.println(describe("x"));
        try {
            System.out.println(area(null));
        } catch (NullPointerException e) {
            System.out.println("npe");
        }
    }
}
