public class VarargsMostSpecific {
  static String select(Object... values) { return "object-varargs"; }
  static String select(String... values) { return "string-varargs"; }
  static String prefix(Object first, Object... rest) { return "object-prefix"; }
  static String prefix(String first, Object... rest) { return "string-prefix"; }
  public static void main(String[] args) {
    System.out.println(select("x"));
    System.out.println(prefix("x"));
  }
}
