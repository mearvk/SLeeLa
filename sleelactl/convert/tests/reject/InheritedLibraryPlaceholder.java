// expect: library-stub
// PIPE_SIZE is inherited from java.io.PipedInputStream, whose lib/java/jdk-28 unit
// declares it only as a placeholder (`static int PIPE_SIZE = 0;`; the JDK value is 1024).
import java.io.PipedInputStream;

public class InheritedLibraryPlaceholder extends PipedInputStream {
    public static void main(String[] args) {
        System.out.println(PIPE_SIZE + 0);
    }
}
