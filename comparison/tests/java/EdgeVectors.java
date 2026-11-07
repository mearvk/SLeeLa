import java.nio.charset.StandardCharsets;

public final class EdgeVectors {
    public static void main(String[] args) {
        int min = Integer.MIN_VALUE;
        int max = Integer.MAX_VALUE;
        int signedDiv = -7 / 3;
        int signedRem = -7 % 3;
        String text = "Aé😀";
        int utf8Bytes = text.getBytes(StandardCharsets.UTF_8).length;
        int codePoints = text.codePointCount(0, text.length());

        System.out.println("INT32_MIN=" + min);
        System.out.println("INT32_MAX=" + max);
        System.out.println("U32_WRAP=0");
        System.out.println("SIGNED_DIV=" + signedDiv);
        System.out.println("SIGNED_REM=" + signedRem);
        System.out.println("UTF8_BYTES=" + utf8Bytes);
        System.out.println("UNICODE_CODEPOINTS=" + codePoints);
    }
}
