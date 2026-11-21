// expect: unboxing-cast
// (int) o with o an Object is a checked cast to Integer followed by unboxing; a String would
// throw ClassCastException. Primitive storage cannot make that check, so the cast is refused.
public class UnboxingCast {
    public static void main(String[] args) {
        Object o = 3;
        int n = (int) o;
        System.out.println(n);
    }
}
