// expect: library-stub
// A project interface that extends a lib/java/jdk-28 interface: reversed() is reached through it on a
// receiver typed as the project interface, and the Comparator unit declares it only as a stub.
import java.util.Comparator;

public class InheritedInterfaceStub {
    interface TextOrder extends Comparator<String> { }

    static class ByLength implements TextOrder {
        public int compare(String a, String b) { return a.length() - b.length(); }
    }

    public static void main(String[] args) {
        TextOrder order = new ByLength();
        System.out.println(order.reversed().compare("a", "bb"));
    }
}
