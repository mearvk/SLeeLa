// expect: library-stub
// Comparator.reversed() is a default method that the lib/java/jdk-28 Comparator unit declares only
// as a stub returning null. An unqualified call inherited through the interface was emitted unchecked.
import java.util.Comparator;

public class InterfaceDefaultStub {
    static class ByLength implements Comparator<String> {
        public int compare(String a, String b) { return a.length() - b.length(); }
        Comparator<String> backwards() { return reversed(); }
    }

    public static void main(String[] args) {
        System.out.println(new ByLength().backwards().compare("a", "bb"));
    }
}
