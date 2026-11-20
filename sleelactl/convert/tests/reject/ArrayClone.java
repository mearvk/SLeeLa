// expect: object-method
public class ArrayClone {
    public static void main(String[] args) {
        int[] a = {1, 2, 3};
        int[] b = a.clone();
        System.out.println(b[0]);
    }
}
