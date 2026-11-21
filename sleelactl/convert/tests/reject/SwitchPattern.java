// expect: switch-pattern
// Type patterns are converted (test PatternSwitch). A guarded pattern (`when`) is not.
public class SwitchPattern {
    public static void main(String[] args) {
        Object value = "x";
        switch (value) {
            case String text when text.isEmpty() -> System.out.println("empty");
            case String text -> System.out.println(text);
            default -> System.out.println("other");
        }
    }
}
