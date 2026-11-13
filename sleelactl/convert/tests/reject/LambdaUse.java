// expect: lambda
public class LambdaUse { public static void main(String[] a) { Runnable r = () -> System.out.println("x"); r.run(); } }
