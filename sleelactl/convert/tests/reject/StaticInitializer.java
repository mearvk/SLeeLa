// expect: init-block
// Class initialization is triggered by Java active-use rules; eager lowering is not equivalent.
public class StaticInitializer {
    static int value;
    static { value = 7; }
    public static void main(String[] args) { System.out.println(value); }
}
