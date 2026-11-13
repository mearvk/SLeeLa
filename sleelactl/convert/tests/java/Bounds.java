public class Bounds {
    static int[] data = {10, 20, 30};
    static int read(int[] a, int i) {
        try { return a[i]; } catch (ArrayIndexOutOfBoundsException e) { System.out.println("read " + e.getMessage()); return -1; }
    }
    static void write(int[] a, int i, int v) {
        try { a[i] = v; } catch (ArrayIndexOutOfBoundsException e) { System.out.println("write " + e.getMessage()); }
    }
    static int make(int n) {
        try { int[] t = new int[n]; return t.length; } catch (NegativeArraySizeException e) { System.out.println("size " + e.getMessage()); return -1; }
    }
    public static void main(String[] args) {
        System.out.println(read(data, 0) + " " + read(data, 2));
        System.out.println(read(data, 3) + " " + read(data, -1));
        write(data, 1, 99);
        write(data, 3, 5);
        write(data, -2, 5);
        System.out.println(data[1] + " " + make(4) + " " + make(-7) + " " + make(0));
        int[][] g = new int[2][3];
        try { g[1][3] = 1; } catch (ArrayIndexOutOfBoundsException e) { System.out.println("inner " + e.getMessage()); }
        try { g[2][0] = 1; } catch (ArrayIndexOutOfBoundsException e) { System.out.println("outer " + e.getMessage()); }
        try { int[][] bad = new int[2][-1]; } catch (NegativeArraySizeException e) { System.out.println("grid " + e.getMessage()); }
        int[] a = new int[4];
        int i = 0;
        a[i] += 5; a[3] *= 2;
        try { a[4] += 1; } catch (ArrayIndexOutOfBoundsException e) { System.out.println("compound " + e.getMessage()); }
        char[] cs = new char[2];
        try { cs[2] = 'x'; } catch (RuntimeException e) { System.out.println("char " + e.getMessage()); }
        try { for (int k = 0; k <= a.length; k++) { a[k] = k; } } catch (IndexOutOfBoundsException e) { System.out.println("loop " + e.getMessage() + " " + a[3]); }
        int total = 0;
        for (int k = 0; k < a.length; k++) { total += a[k]; }
        System.out.println(total);
    }
}
