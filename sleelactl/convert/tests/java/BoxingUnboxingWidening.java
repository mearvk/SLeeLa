public class BoxingUnboxingWidening {
  static long widen(long value) { return value; }
  static Number reference(Number value) { return value; }
  static String choose(Object value) { return "object"; }
  static String choose(Number value) { return "number"; }
  public static void main(String[] args) {
    Integer value = 19;
    System.out.println(widen(value));
    System.out.println(reference(value));
    System.out.println(choose(value));
  }
}
