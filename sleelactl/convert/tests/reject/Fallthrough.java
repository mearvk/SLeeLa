// expect: switch-fallthrough
public class Fallthrough { public static void main(String[] a) { int n = 1; switch (n) { case 1: System.out.println("a"); case 2: System.out.println("b"); break; default: System.out.println("c"); } } }
