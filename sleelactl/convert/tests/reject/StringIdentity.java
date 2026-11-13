// expect: string-identity
public class StringIdentity { public static void main(String[] a) { String s = "x"; String t = a.length == 0 ? "x" : "y"; System.out.println(s == t); } }
