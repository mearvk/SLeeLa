// expect: lambda-target
// The lambda goes to a library constructor whose signature was not collected, so its target interface is unknown.
// (Until 2026-11-20 this fixture called java.util.List.forEach; that call is now refused first,
// as [library-member], because the lib/java List unit does not declare forEach.)
public class LambdaUse { public static void main(String[] a) { Thread t = new Thread(() -> System.out.println(1)); } }
