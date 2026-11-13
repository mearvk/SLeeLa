// expect: labeled
public class LabeledLoop { public static void main(String[] a) { outer: for (int i = 0; i < 2; i++) { for (int j = 0; j < 2; j++) { if (j == 1) break outer; } } } }
