// expect: switch-pattern
// A pattern switch is lowered to a loop that runs once. An unlabeled continue in an arm would
// restart that loop instead of the enclosing Java loop, so it is refused.
public class SwitchPatternContinue {
    public static void main(String[] args) {
        Object[] values = { "a", 1 };
        int n = 0;
        for (Object v : values) {
            switch (v) {
                case String s -> { continue; }
                default -> n++;
            }
        }
        System.out.println(n);
    }
}
