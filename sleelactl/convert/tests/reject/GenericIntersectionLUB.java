// expect: generic-inference
public class GenericIntersectionLUB {
    interface Left { int left(); }
    interface Right { int right(); }

    static class First implements Left, Right {
        public int left() { return 1; }
        public int right() { return 2; }
    }

    static class Second implements Left, Right {
        public int left() { return 3; }
        public int right() { return 4; }
    }

    static <T> T choose(T a, T b) {
        return a;
    }

    public static void main(String[] args) {
        System.out.println(choose(new First(), new Second()).right());
    }
}
