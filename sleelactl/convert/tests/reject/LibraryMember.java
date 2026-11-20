// expect: library-member
// The lib/java/jdk-28 Optional unit declares empty() as an instance method and has no no-argument
// constructor, so neither Optional.empty() nor new Optional().empty() resolves.
import java.util.Optional;

public class LibraryMember {
    public static void main(String[] args) {
        Optional<String> none = Optional.empty();
        System.out.println(none.isPresent());
    }
}
