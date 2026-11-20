// expect: library-stub
// ArrayList.removeIf is declared in the lib/java/jdk-28 ArrayList unit only as a signature stub that
// returns a default value; a converted call would compile but would not remove anything.
import java.util.ArrayList;

public class LibraryStub {
    public static void main(String[] args) {
        ArrayList<String> list = new ArrayList<>();
        list.add("");
        list.removeIf(s -> s.isEmpty());
        System.out.println(list.size());
    }
}
