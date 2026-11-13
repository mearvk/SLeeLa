// expect: untyped
public class Untyped { static Object get() { return null; } public static void main(String[] a) { int x = unknown() + 1; System.out.println(x); } }
