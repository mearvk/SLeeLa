// expect: synchronized
public class SyncBlock { static Object o = new Object(); public static void main(String[] a) { synchronized (o) { System.out.println(1); } } }
