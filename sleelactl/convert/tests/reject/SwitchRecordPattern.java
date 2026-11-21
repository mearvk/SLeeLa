// expect: switch-pattern
// Record deconstruction patterns (JLS 14.30.1) are not converted.
public class SwitchRecordPattern {
    record Point(int x, int y) { }

    public static void main(String[] args) {
        Object o = new Point(1, 2);
        switch (o) {
            case Point(int x, int y) -> System.out.println(x + y);
            default -> System.out.println("other");
        }
    }
}
