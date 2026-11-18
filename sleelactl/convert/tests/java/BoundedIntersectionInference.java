public class BoundedIntersectionInference {
  static <T extends Number & Comparable<T>> T retain(T value) {
    return value;
  }
  public static void main(String[] args) {
    Integer value = retain(42);
    System.out.println(value);
  }
}
