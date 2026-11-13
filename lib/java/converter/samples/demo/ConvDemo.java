package demo;

/* Converter differential: plain procedural Java. */
public class ConvDemo {
    static final int[] PRIMES = {2, 3, 5, 7, 11};
    static int counter;

    static {
        counter = 40;
    }

    enum Color { RED, GREEN, BLUE }

    static int hash(String s) {
        int h = 0;
        for (int i = 0; i < s.length(); i++) {
            h = 31 * h + s.charAt(i);
        }
        return h;
    }

    static long sum(int[] xs) {
        long s = 0;
        for (int x : xs) s += x;
        return s;
    }

    static String kind(int n) {
        switch (n % 3) {
            case 0: return "fizz";
            case 1: return "one";
            default: return "two";
        }
    }

    public static void main(String[] args) {
        int big = 2147483647;
        big = big + 1;
        System.out.println("wrap " + big);
        System.out.println("hex " + 0xFFFFFFFF + " " + 0x7f + " oct " + 017 + " bin " + 0b1010);
        System.out.println("hash " + hash("hello world"));
        System.out.println("sum " + sum(PRIMES) + " len " + PRIMES.length);
        int[] zeros = new int[4];
        System.out.println("zero " + zeros[3]);
        int a = 3, b = 4;
        System.out.println("decl " + (a * b));
        System.out.println("char " + 'x' + " code " + (int) 'A');
        byte by = (byte) 200;
        short sh = (short) 70000;
        System.out.println("narrow " + by + " " + sh);
        for (int i = 0; i < 4; i++) System.out.println("kind " + i + " " + kind(i));
        System.out.println("enum " + Color.GREEN.name() + " " + Color.BLUE.ordinal() + " " + Color.values().length);
        System.out.println("static " + counter);
        int k = 0;
        while (k < 1000) k = k * 2 + 1;
        System.out.println("loop " + k + " shift " + (-16 >>> 28) + " " + (1 << 33));
        String t = """
            alpha
              beta
            """;
        System.out.println("text [" + t.trim() + "] " + t.length());
        System.out.println(k > 500 ? "big" : "small");
        // syntax 3.1 forms: comma for headers, labels, finally on return, nested initializers
        int acc = 0;
        for (int i = 0, j = 10; i < j; i++, j--) {
            if (i == 2) continue;
            if (i == 4) break;
            acc += i * j;
        }
        System.out.println("comma-for " + acc);
        int found = -1;
        outer:
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                if (c > r) continue outer;
                if (r * c == 6) { found = r * 10 + c; break outer; }
            }
        }
        System.out.println("label " + found);
        System.out.println("finally " + guarded(3) + " " + trace);
        int[][] grid = {{1, 2, 3}, {4, 5}};
        int[] row = grid[1];
        System.out.println("grid " + grid.length + " " + row[1]);
        char c = 'q';
        switch (c) {
            case 'p': System.out.println("p"); break;
            case 'q': System.out.println("is q"); break;
            default: System.out.println("other");
        }
        System.out.println("pattern " + describe("abc") + ", " + describe(Integer.valueOf(9)) + ", " + describe(Integer.valueOf(2)) + ", " + emptyText("") + " " + emptyText(Integer.valueOf(1)));
    }

    static String describe(Object o) {
        if (o instanceof String s) {
            return "str " + s.length();
        }
        if (o instanceof Integer n && n.intValue() > 5) {
            return "big " + n.intValue();
        }
        return "other";
    }

    static boolean emptyText(Object o) {
        return o instanceof String s && s.isEmpty();
    }

    static String trace = "";

    static int guarded(int n) {
        try {
            if (n > 2) return n * 2;
            return n;
        } finally {
            trace = trace + "ran";
        }
    }
}
