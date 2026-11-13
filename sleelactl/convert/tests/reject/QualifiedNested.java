// expect: object-method
// Regression (2026-11-13): Outer.Inner in a type names the hoisted Outer__Inner. Resolving only the last
// segment bound Fmt.Field to this file's other nested Field, so QualifiedNested.Field extended itself (as
// java.text.NumberFormat.Field extends Format.Field did) and walking its superclass chain never ended.
public class QualifiedNested {
    static class Fmt {
        static class Field { }
    }
    public static class Field extends Fmt.Field {
        protected Field() {
            if (this.getClass() == null) { }
        }
    }
    public static void main(String[] args) { }
}
