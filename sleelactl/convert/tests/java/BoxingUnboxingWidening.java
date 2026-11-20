public class BoxingUnboxingWidening {
  static long widen(long value) { return value; }
  static String choose(Object value) { return "object"; }
  static String choose(long value) { return "long"; }
  // Integer -> Number (a wrapper to a class supertype) is refused: see reject/BoxedToNumber.
  public static void main(String[] args) {
    Integer value = 19;
    System.out.println(widen(value));
    System.out.println(choose(value));
  }
}
