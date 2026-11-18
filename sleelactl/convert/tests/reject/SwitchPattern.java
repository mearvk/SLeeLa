// expect: switch-pattern
public class SwitchPattern { public static void main(String[] args) { Object value = "x"; switch (value) { case String text -> System.out.println(text); default -> System.out.println("other"); } } }
