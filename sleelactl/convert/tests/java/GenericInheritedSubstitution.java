public class GenericInheritedSubstitution {
  interface Value<T> { T get(); }
  static class Base<T> implements Value<T> {
    private T value;
    Base(T value) { this.value = value; }
    public T get() { return value; }
  }
  static class TextValue extends Base<String> {
    TextValue(String value) { super(value); }
  }
  static class IntegerValue extends Base<Integer> {
    IntegerValue(Integer value) { super(value); }
  }
  public static void main(String[] args) {
    Value<String> text = new TextValue("inherited-substitution");
    Value<Integer> number = new IntegerValue(41);
    System.out.println(text.get());
    System.out.println(number.get());
  }
}
