public class BoundedGenericIntersection {
    interface Left { int left(); }
    interface Right { int right(); }

    static class Both implements Left, Right {
        public int left() { return 2; }
        public int right() { return 3; }
    }

    static <T extends Left & Right> T keep(T value) {
        return value;
    }

    public static void main(String[] args) {
        Both value = keep(new Both());
        System.out.println(value.left() + value.right());
    }
}
