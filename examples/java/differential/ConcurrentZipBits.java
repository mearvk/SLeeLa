import java.util.*;
import java.util.concurrent.atomic.*;
import java.util.zip.*;

/** Java side of the ConcurrentZipBits differential test; prints one result per line. */
public class ConcurrentZipBits {
    public static void main(String[] args) {
        AtomicInteger ai = new AtomicInteger(2147483646);
        System.out.println(ai.incrementAndGet() + " " + ai.incrementAndGet() + " " + ai.getAndAdd(5) + " " + ai.get());
        System.out.println(ai.compareAndSet(-2147483643, 7) + " " + ai.compareAndSet(1, 2) + " " + ai.compareAndExchange(7, 9) + " " + ai);
        AtomicLong al = new AtomicLong(40);
        System.out.println(al.addAndGet(2) + " " + al.getAndSet(-1) + " " + al.decrementAndGet() + " " + al.intValue() + " " + al.doubleValue());
        AtomicBoolean ab = new AtomicBoolean(false);
        System.out.println(ab.compareAndSet(false, true) + " " + ab.get() + " " + ab.getAndSet(false) + " " + ab);
        AtomicReference<String> ar = new AtomicReference<>("a");
        System.out.println(ar.compareAndSet("a", "b") + " " + ar.get() + " " + ar.getAndSet(null) + " " + ar);
        AtomicIntegerArray aia = new AtomicIntegerArray(3);
        aia.set(1, 5); aia.addAndGet(2, 7); aia.getAndIncrement(0);
        System.out.println(aia + " " + aia.length() + " " + aia.compareAndSet(1, 5, 6) + " " + aia.get(1));
        try { aia.get(3); } catch (IndexOutOfBoundsException e) { System.out.println(e.getClass().getName() + ": " + e.getMessage()); }
        AtomicLongArray ala = new AtomicLongArray(2);
        ala.set(0, 9000000000L); System.out.println(ala + " " + ala.incrementAndGet(0));
        AtomicReferenceArray<String> ara = new AtomicReferenceArray<>(2);
        ara.set(0, "x"); System.out.println(ara);
        LongAdder la = new LongAdder(); la.add(10); la.increment(); la.decrement(); la.add(-3);
        System.out.println(la.sum() + " " + la + " " + la.sumThenReset() + " " + la.sum());
        DoubleAdder da = new DoubleAdder(); da.add(0.1); da.add(0.2);
        System.out.println(da.sum() + " " + da);
        IntSummaryStatistics is = new IntSummaryStatistics();
        System.out.println(is);
        is.accept(3); is.accept(-7); is.accept(10);
        System.out.println(is + " " + is.getAverage());
        LongSummaryStatistics ls = new LongSummaryStatistics(); ls.accept(5000000000L); ls.accept(1);
        System.out.println(ls);
        DoubleSummaryStatistics ds = new DoubleSummaryStatistics();
        for (int i = 0; i < 10; i++) ds.accept(0.1);
        ds.accept(-2.5);
        System.out.println(ds.getSum() + " " + ds.getAverage() + " " + ds);
        int[] data = new int[9];
        String s = "123456789";
        for (int i = 0; i < 9; i++) data[i] = s.charAt(i);
        byte[] b = new byte[9];
        for (int i = 0; i < 9; i++) b[i] = (byte) data[i];
        CRC32 c = new CRC32(); c.update(b, 0, 9);
        CRC32C cc = new CRC32C(); cc.update(b, 0, 9);
        Adler32 ad = new Adler32(); ad.update(b, 0, 9);
        System.out.println(c.getValue() + " " + cc.getValue() + " " + ad.getValue());
        c.reset(); c.update(255); c.update(-1); System.out.println(c.getValue());
        BitSet bs = new BitSet(10);
        bs.set(1); bs.set(3); bs.set(64); bs.set(100, 103); bs.clear(101);
        System.out.println(bs + " " + bs.cardinality() + " " + bs.length() + " " + bs.size() + " " + bs.nextSetBit(4) + " " + bs.nextClearBit(1) + " " + bs.previousSetBit(99));
        System.out.println(bs.hashCode() + " " + Arrays.toString(bs.toLongArray()) + " " + Arrays.toString(bs.toByteArray()));
        BitSet other = new BitSet(); other.set(3); other.set(5);
        BitSet x = (BitSet) bs.clone(); x.and(other); BitSet y = (BitSet) bs.clone(); y.xor(other);
        System.out.println(x + " " + y + " " + bs.intersects(other) + " " + bs.get(1, 4) + " " + BitSet.valueOf(new long[]{-1L, 5L}).cardinality());
        BitSet neg = new BitSet(); neg.set(63); System.out.println(neg.hashCode() + " " + Arrays.toString(neg.toLongArray()));
        StackTraceElement st = new StackTraceElement("com.example.Main", "run", "Main.java", 42);
        StackTraceElement nat = new StackTraceElement("java.lang.Thread", "sleep0", null, -2);
        System.out.println(st + " " + nat + " " + new StackTraceElement("A", "b", "A.java", -1) + " " + new StackTraceElement("A", "b", null, 3));
        System.out.println(st.hashCode() + " " + st.equals(new StackTraceElement("com.example.Main", "run", "Main.java", 42)) + " " + nat.isNativeMethod());
    }
}
