public class Strings {
    public static void main(String[] args) {
        String s = "Hello, World";
        System.out.println(s.length());
        System.out.println(s.charAt(4));
        System.out.println(s.substring(7));
        System.out.println(s.indexOf("World"));
        System.out.println(s.toUpperCase());
        System.out.println(s.equals("Hello, World"));
        char c = s.charAt(0);
        System.out.println("c=" + c + " " + (int) c + " " + (char) (c + 1));
        int n = 0;
        for (int i = 0; i < s.length(); i++) { if (s.charAt(i) == 'o') { n++; } }
        System.out.println("o count " + n);
        double d = 10;
        System.out.println("d=" + d + " " + (d / 4) + " " + 1e21 + " " + 1.0E-5);
        System.out.println(100 + 200 + "x" + 100 + 200);
        System.out.println("t" + true + 'c' + 1L);
    }
}
