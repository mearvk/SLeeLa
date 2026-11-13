public class Flow {
    static int[] primes = {2, 3, 5, 7, 11};
    static int f1() { try { System.out.println("try1"); return 1; } finally { System.out.println("fin1"); } }
    static int f2(int x) {
        try {
            if (x > 0) { return 10; }
            throw new IllegalStateException("boom");
        } catch (IllegalStateException e) {
            System.out.println("catch2");
            return 20;
        } finally {
            System.out.println("fin2");
        }
    }
    static int f3() { try { return 1; } finally { System.out.println("fin3"); return 3; } }
    static int f4() {
        try { try { return 4; } finally { System.out.println("inner4"); } }
        finally { System.out.println("outer4"); }
    }
    static int f5() {
        int i = 0;
        while (i < 5) {
            i = i + 1;
            try {
                if (i == 2) { continue; }
                if (i == 4) { break; }
                System.out.println("body5 " + i);
            } finally { System.out.println("fin5 " + i); }
        }
        return i;
    }
    static int labels() {
        int found = 0;
        outer:
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (j == 3) { continue outer; }
                if (i == 3) { break outer; }
                found++;
            }
        }
        return found;
    }
    static int sum(int[] xs) { int r = 0; for (int x : xs) { r += x; } return r; }
    public static void main(String[] args) {
        System.out.println(f1());
        System.out.println(f2(1));
        System.out.println(f2(0));
        System.out.println(f3());
        System.out.println(f4());
        System.out.println(f5());
        System.out.println(labels());
        System.out.println(sum(primes) + " " + primes.length);
        int[] local = {5, 6, 7};
        double[] ds = {1, 2.5};
        String[] names = new String[]{"a", "b", "c"};
        System.out.println(sum(local) + " " + ds[0] / 2 + " " + names[2] + names.length);
        System.out.println(sum(new int[]{1, 2, 3, 4}));
    }
}
