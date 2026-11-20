// A single static import of a /lib field (JLS 7.5.3): the bare NAME is read as
// TimeUnit.NANOSECONDS. It was refused as [library-type] (taken for a type name).
import static java.util.concurrent.TimeUnit.NANOSECONDS;
import java.util.concurrent.TimeUnit;

public class LibraryStaticImport {
    public static void main(String[] args) {
        TimeUnit unit = NANOSECONDS;
        System.out.println(unit.name());
        System.out.println(unit == TimeUnit.NANOSECONDS);
        System.out.println(NANOSECONDS.ordinal());
    }
}
