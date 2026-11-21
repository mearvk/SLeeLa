// An array of arrays initialized from array expressions (JLS 10.6): the element references are
// stored, so a later write through an element shows through the outer array.
public class ArrayOfArrays {
    public static void main(String[] args) {
        int[] r1 = {1, 2};
        int[] r2 = {3};
        int[][] m = {r1, r2, {4, 5, 6}, null};
        System.out.println(m.length + " " + m[2][1] + " " + (m[3] == null));
        r1[0] = 9;
        System.out.println(m[0][0]);
        byte[] t1 = {1}, t2 = {2, 3};
        byte[][][] cube = new byte[][][] { {t1, t2}, {t2} };
        System.out.println(cube[0][1][1] + " " + cube[1].length);
    }
}
