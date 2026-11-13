import java.io.*;
import java.util.*;

/** Java side of the Jdk28Throwables differential test (java.* exceptions, JDK 28 tier). */
public class Jdk28Throwables {
    public static void main(String[] args) {
        IllegalArgumentException a = new IllegalArgumentException("bad");
        System.out.println(a + " | " + a.getMessage() + " | " + (a.getCause() == null));
        IllegalStateException s = new IllegalStateException("wrapped", new ArithmeticException("div"));
        System.out.println(s + " | " + s.getCause());
        RuntimeException r = new RuntimeException();
        System.out.println(r + " | " + r.getMessage() + " | " + r.getLocalizedMessage());
        IOException root = new IOException("disk full");
        IOException w = new IOException(root);
        System.out.println(w.getMessage() + " | " + (w.getCause() == root));
        IOException both = new IOException("copy failed", root);
        System.out.println(both + " | " + both.getCause());
        NoSuchElementException ns = new NoSuchElementException("gone");
        System.out.println(ns + " | " + (ns instanceof RuntimeException) + " " + (ns instanceof Exception));
        UnsupportedOperationException un = new UnsupportedOperationException((Throwable) null);
        System.out.println(un.getMessage() + " | " + (un.getCause() == null));
        Exception e = new Exception("first");
        try { e.initCause(root); e.initCause(root); } catch (IllegalStateException ise) { System.out.println(ise.getMessage()); }
        try { new Exception("x").initCause(null); System.out.println("null cause ok"); } catch (IllegalStateException ise) { System.out.println(ise.getMessage()); }
        Exception self = new Exception("self");
        try { self.initCause(self); } catch (IllegalArgumentException iae) { System.out.println(iae.getMessage()); }
        Exception sup = new Exception("main");
        sup.addSuppressed(new IllegalStateException("one"));
        sup.addSuppressed(new IOException("two"));
        Throwable[] list = sup.getSuppressed();
        System.out.println(list.length + " " + list[0] + " " + list[1]);
        try { sup.addSuppressed(sup); } catch (IllegalArgumentException iae) { System.out.println(iae.getMessage()); }
        try { sup.addSuppressed(null); } catch (NullPointerException npe) { System.out.println(npe.getMessage()); }
        Error err = new Error("fatal", new RuntimeException("cause"));
        System.out.println(err + " | " + err.getCause().getMessage());
        System.out.println(new StackOverflowError() + " | " + new ArrayIndexOutOfBoundsException("idx") + " | " + new NumberFormatException("nf"));
        System.out.println((new NumberFormatException("q") instanceof IllegalArgumentException) + " " + (new FileNotFoundException("f") instanceof IOException));
        Throwable t = new Throwable("plain");
        System.out.println(t);
    }
}
