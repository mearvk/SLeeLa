public class IdentifierParity {
    int next = 3;

    int next() {
        return 4;
    }

    public static void main(String[] args) {
        int next = 5;
        int String = 6;
        IdentifierParity x = new IdentifierParity();
        System.out.println(next + String + x.next() + x.next);
    }
}
