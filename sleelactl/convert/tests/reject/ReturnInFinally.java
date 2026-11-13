// expect: return-in-finally
public class ReturnInFinally { static int f() { try { return 1; } finally { System.out.println("x"); } } public static void main(String[] a) { f(); } }
