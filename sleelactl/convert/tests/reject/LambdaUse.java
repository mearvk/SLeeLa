// expect: lambda-target
public class LambdaUse { public static void main(String[] a) { java.util.List<String> l = null; l.forEach(s -> System.out.println(s)); } }
