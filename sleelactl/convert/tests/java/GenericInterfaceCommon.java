public class GenericInterfaceCommon {
    interface Tagged {
        int tag();
    }

    static class First implements Tagged {
        public int tag() { return 4; }
    }

    static class Second implements Tagged {
        public int tag() { return 5; }
    }

    static <T> T choose(T first, T second) {
        return first;
    }

    static int read(Tagged value) {
        return value.tag();
    }

    public static void main(String[] args) {
        System.out.println(read(choose(new First(), new Second())));
    }
}
