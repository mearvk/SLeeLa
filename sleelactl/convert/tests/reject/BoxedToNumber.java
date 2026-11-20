// expect: boxed-reference
// Boxed values use primitive storage in the Java tier. Object (the VM universal
// type) can hold one; lib/java Number, a SLeeLa class type, cannot.
public class BoxedToNumber {
  static Number reference(Number value) { return value; }
  public static void main(String[] args) {
    Integer value = 19;
    System.out.println(reference(value));
  }
}
