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
        // Shared-model regression coverage: binary numeric promotion and the
        // primitive cast matrix, especially conversions to/from binary32.
        System.out.println("mixed " + (1 + 2.5f) + " " + (2L + 0.5f) + " " + (2.0 + 0.5f));
        byte by = -7;
        short sh2 = 32001;
        char ch2 = 65530;
        int in = 16777217;
        long lo = 9007199254740993L;
        float fl = 0.1f;
        double db = 0.1d;
        System.out.println("cast-a " + (short) by + " " + (char) by + " " + (float) by + " " + (double) by);
        System.out.println("cast-b " + (byte) sh2 + " " + (char) sh2 + " " + (float) sh2 + " " + (double) sh2);
        System.out.println("cast-c " + (byte) ch2 + " " + (short) ch2 + " " + (float) ch2 + " " + (double) ch2);
        System.out.println("cast-d " + (byte) in + " " + (short) in + " " + (char) in + " " + (float) in);
        System.out.println("cast-e " + (byte) lo + " " + (short) lo + " " + (char) lo + " " + (float) lo);
        System.out.println("cast-f " + (byte) fl + " " + (short) fl + " " + (int) fl + " " + (long) fl + " " + (double) fl);
        System.out.println("cast-g " + (byte) db + " " + (short) db + " " + (char) db + " " + (int) db + " " + (long) db + " " + (float) db);
        System.out.println("float-unary " + (+fl) + " " + (-fl) + " " + (fl % 0.03f));
    }
}
