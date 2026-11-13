public class Grid {
    static int[][] identity(int n) {
        int[][] g = new int[n][n];
        for (int i = 0; i < n; i++) { g[i][i] = 1; }
        return g;
    }
    static int sum(int[][] g) {
        int s = 0;
        for (int i = 0; i < g.length; i++) { for (int j = 0; j < g[i].length; j++) { s += g[i][j]; } }
        return s;
    }
    static String text(int[][] g) {
        String t = "";
        for (int[] row : g) {
            for (int v : row) { t += v; t += " "; }
            t += "|";
        }
        return t;
    }
    public static void main(String[] args) {
        int[][] id = identity(4);
        System.out.println(sum(id) + " " + id.length + " " + id[0].length);
        System.out.println(text(id));
        int[][] lit = {{1, 2}, {3, 4, 5}, {}};
        System.out.println(sum(lit) + " " + lit[1].length + " " + lit[2].length);
        double[][] d = new double[2][3];
        d[1][2] = 1.5;
        System.out.println(d[1][2] + d[0][0]);
        String[][] s = new String[2][2];
        System.out.println(s[1][1] == null);
        int[][][] cube = new int[2][3][4];
        cube[1][2][3] = 9;
        System.out.println(cube[1][2][3] + cube.length * cube[0].length * cube[0][0].length);
        int[][] jag = new int[3][];
        for (int i = 0; i < jag.length; i++) { jag[i] = new int[i + 1]; jag[i][i] = i; }
        System.out.println(sum(jag) + " " + jag[2].length);
        char[][] cg = new char[2][2];
        cg[0][1] = 'x';
        System.out.println(cg[0][1]);
        int[][] viaNew = new int[][]{{7, 8}, {9}};
        System.out.println(viaNew[0][1] + viaNew[1][0]);
        long total = 0;
        for (int k = 0; k < 300; k++) { int[][] t = new int[40][40]; t[39][39] = k; total += t[39][39]; }
        System.out.println(total);
    }
}
