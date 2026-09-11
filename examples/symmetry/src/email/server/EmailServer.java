package symmetry.email.server;

import java.util.ArrayDeque;
import java.util.Deque;

public final class EmailServer {
    private final Deque<String> queue = new ArrayDeque<>();
    private long deliveredCount;
    public void submit(String message) { queue.addLast(message); deliveredCount++; }
    public String fetch() { return queue.pollFirst(); }
    public long getDeliveredCount() { return deliveredCount; }
    public int getQueueSize() { return queue.size(); }
}
