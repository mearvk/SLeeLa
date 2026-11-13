import java.util.*;
import java.util.concurrent.*;

/** Java side of the ConcurrentCollections differential test. */
public class ConcurrentCollections {
    public static void main(String[] args) throws Exception {
        ConcurrentHashMap<Object, Object> chm = new ConcurrentHashMap<>();
        chm.put("a", 1); chm.put("b", 2); chm.putIfAbsent("a", 9); chm.put("c", 3); chm.remove("b");
        System.out.println(chm.size() + " " + chm.get("a") + " " + chm.get("zz") + " " + chm.containsKey("c") + " " + chm.contains(3) + " " + chm.mappingCount() + " " + chm.getOrDefault("q", 7));
        try { chm.put(null, 1); } catch (NullPointerException e) { System.out.println("NPE key"); }
        try { chm.put("k", null); } catch (NullPointerException e) { System.out.println("NPE value"); }
        Hashtable<Object, Object> ht = new Hashtable<>();
        ht.put("x", 10); ht.put("y", 20);
        Enumeration<Object> en = ht.elements(); int sum = 0; while (en.hasMoreElements()) sum += (Integer) en.nextElement();
        System.out.println(ht.size() + " " + ht.get("x") + " " + sum + " " + ht.contains(20) + " " + ht.containsKey("y"));
        CopyOnWriteArrayList<Object> cow = new CopyOnWriteArrayList<>();
        cow.add("p"); cow.add("q"); System.out.println(cow.addIfAbsent("p") + " " + cow.addIfAbsent("r") + " " + cow + " " + cow.addAllAbsent(List.of("q", "s", "t")) + " " + cow);
        CopyOnWriteArraySet<Object> cows = new CopyOnWriteArraySet<>();
        cows.add(3); cows.add(1); cows.add(3); System.out.println(cows + " " + cows.size());
        ArrayBlockingQueue<Object> abq = new ArrayBlockingQueue<>(2);
        System.out.println(abq.offer(1) + " " + abq.offer(2) + " " + abq.offer(3) + " " + abq.remainingCapacity() + " " + abq.poll() + " " + abq.peek() + " " + abq.size());
        try { abq.add(5); abq.add(6); } catch (IllegalStateException e) { System.out.println(e.getMessage()); }
        LinkedBlockingQueue<Object> lbq = new LinkedBlockingQueue<>();
        lbq.put("j1"); lbq.put("j2"); lbq.put("j3");
        ArrayList<Object> sink = new ArrayList<>();
        System.out.println(lbq.take() + " " + lbq.drainTo(sink) + " " + sink + " " + lbq.poll(5, TimeUnit.MILLISECONDS));
        LinkedBlockingDeque<Object> lbd = new LinkedBlockingDeque<>();
        lbd.putFirst(2); lbd.putFirst(1); lbd.putLast(3); System.out.println(lbd + " " + lbd.takeLast() + " " + lbd.takeFirst() + " " + lbd);
        ConcurrentLinkedQueue<Object> clq = new ConcurrentLinkedQueue<>();
        clq.offer("m"); clq.offer("n"); System.out.println(clq.poll() + " " + clq.peek() + " " + clq.size() + " " + clq.isEmpty());
        ConcurrentLinkedDeque<Object> cld = new ConcurrentLinkedDeque<>();
        cld.push(1); cld.push(2); cld.offerLast(3); System.out.println(cld + " " + cld.pop() + " " + cld.pollLast() + " " + cld);
        ConcurrentSkipListMap<Object, Object> sl = new ConcurrentSkipListMap<>();
        sl.put("pear", 1); sl.put("apple", 2); sl.put("fig", 3);
        System.out.println(sl + " " + sl.firstKey() + " " + sl.ceilingKey("b") + " " + sl.headMap("g"));
        ConcurrentSkipListSet<Object> ss = new ConcurrentSkipListSet<>();
        ss.add(5); ss.add(2); ss.add(9); ss.add(2); System.out.println(ss + " " + ss.first() + " " + ss.higher(5) + " " + ss.pollFirst() + " " + ss);
    }
}
