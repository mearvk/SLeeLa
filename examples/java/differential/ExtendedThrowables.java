import javax.crypto.*;
import javax.naming.*;
import javax.print.*;
import javax.xml.stream.*;
import java.security.GeneralSecurityException;

/** Java side of the ExtendedThrowables differential test (javax.* exceptions). */
public class ExtendedThrowables {
    public static void main(String[] args) {
        BadPaddingException bp = new BadPaddingException("pad");
        System.out.println(bp + " | " + bp.getMessage() + " | " + (bp instanceof GeneralSecurityException) + " " + (bp instanceof Exception) + " " + ((Object) bp instanceof RuntimeException));
        AEADBadTagException tag = new AEADBadTagException("tag mismatch");
        System.out.println(tag + " | " + (tag instanceof BadPaddingException));
        NamingException ne = new NamingException("lookup failed");
        System.out.println(ne.getMessage() + " | " + (ne instanceof Exception));
        NameNotFoundException nnf = new NameNotFoundException("jdbc/x");
        System.out.println(nnf.getMessage() + " | " + (nnf instanceof NamingException));
        PrintException pe = new PrintException("no printer");
        System.out.println(pe + " | " + (pe.getCause() == null));
        XMLStreamException xe = new XMLStreamException("bad xml");
        System.out.println(xe.getMessage() + " | " + (xe instanceof Exception));
        NoSuchPaddingException np = new NoSuchPaddingException("PKCS9");
        System.out.println(np + " | " + (np instanceof GeneralSecurityException));
        IllegalBlockSizeException ib = new IllegalBlockSizeException("16");
        ib.initCause(new java.io.IOException("io"));
        System.out.println(ib + " | " + ib.getCause());
    }
}
