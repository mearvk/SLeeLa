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
        java.io.IOException root = new java.io.IOException("disk full");
        java.io.IOException wrapped = new java.io.IOException(root);
        java.io.IOException both = new java.io.IOException("copy failed", root);
        RuntimeException bare = new RuntimeException();
        NamingException nested = new NamingException();
        System.out.println(wrapped.getMessage() + " | " + wrapped.getCause() + " | " + both.getMessage() + " | " + (both.getCause() == root));
        System.out.println(bare + " | " + bare.getMessage() + " | " + (bare.getCause() == null) + " | " + nested.getMessage());
        IllegalStateException st = new IllegalStateException("bad", new ArithmeticException("x"));
        UnsupportedOperationException un = new UnsupportedOperationException((Throwable) null);
        System.out.println(st.getCause() + " | " + un.getMessage() + " | " + new BadPaddingException() + " | " + new java.util.NoSuchElementException("gone", root).getCause());
    }
}
