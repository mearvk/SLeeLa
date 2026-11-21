// expect: unboxing-cast
// The first method is refused (unboxing-cast). Recovery must skip exactly that method, so the next member, whose
// annotation value has braces, is parsed from its start and reports its own refusal (string
// identity) rather than a syntax error inside the annotation. The member-recovery
// check of tests/javaconv_differential_test.sh reads both diagnostics from the report.
public class RecoveryAfterAnnotation {
    static int first() {
        Object o = "x";
        return (int) o;
    }

    @SuppressWarnings({"rawtypes", "unchecked"})
    static boolean second(String a, String b) {
        return a == b;
    }

    public static void main(String[] args) {
        System.out.println(second("a", "a"));
    }
}
