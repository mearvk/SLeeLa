package nextname;

import java.util.ArrayList;
import java.util.Iterator;

/** A variable, parameter and field named next (SLeeLa reads a bare next as 1), and Iterator.next() calls. */
public class NextName {
    static class Node {
        int value;
        Node next;
        Node(int value, Node next) { this.value = value; this.next = next; }
    }
    static int after(int next) { return next + 1; }
    public static void main(String[] args) {
        Node list = new Node(1, new Node(2, new Node(3, null)));
        int sum = 0;
        for (Node n = list; n != null; n = n.next) { sum = sum + n.value; }
        int next = 40;
        next = next + 2;
        System.out.println(sum + " " + next + " " + after(next) + " " + list.next.next.value);
        ArrayList<String> names = new ArrayList<>();
        names.add("a");
        names.add("b");
        Iterator<String> it = names.iterator();
        String all = "";
        while (it.hasNext()) { all = all + it.next(); }
        System.out.println(all);
    }
}
