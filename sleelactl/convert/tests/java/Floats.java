public class Floats {
    static float scale(float x, int k) { return x * k + 0.1f; }
    public static void main(String[] args) {
        float a = 1.1f, b = 2.2f;
        float c = a + b;
        System.out.println(c);
        System.out.println(a * b);
        System.out.println(a / 3);
        System.out.println(scale(a, 7));
        double d = a;
        System.out.println(d);
        float big = 3.4e38f;
        System.out.println(big * 10);
        System.out.println((int) 2.9f + " " + (long) -2.9f + " " + (float) 0.1 + " " + (double) 0.1f);
        float sum = 0;
        for (int i = 0; i < 10; i++) { sum += 0.1f; }
        System.out.println(sum);
        float[] arr = new float[3];
        arr[1] = 16777216f;
        arr[1] += 1;
        System.out.println(arr[1] + " " + arr[0]);
        System.out.println(a < b);
        System.out.println(1 / 3f);
        System.out.println((float) 16777217);
        float f = 5;
        f++;
        System.out.println(f + " " + -f);
        System.out.println("v=" + (a + 1));
        System.out.println(1e-10f * 1e-10f);
    }
}
