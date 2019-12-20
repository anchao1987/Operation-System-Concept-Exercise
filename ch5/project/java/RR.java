import java.util.*;
/**
 * Round-Robin Schedule Algorithm
 * time-quantum is 10 milleseconds
 */
public class RR implements Algorithm {
    private static final int TIMEQUANTUM = 10;
    Map<Integer,Integer> remaindMap;
    private Deque<Task> rrQueue;
    public RR(List<Task> queue) {
        rrQueue = new LinkedList<>();
        rrQueue.addAll(queue);
        remaindMap = new HashMap<>();
        for (Task q : queue) {
            remaindMap.put(q.getTid(),q.getBurst());
        }
    }



        /**
     * Invokes the scheduler
     */
    public void schedule() {
        Task nextTask = pickNextTask();
        if (nextTask == null) {
            return;
        }
        CPU.run(nextTask, 0);
    }

    /**
     * Selects the next task using the appropriate scheduling algorithm
     */
    public Task pickNextTask() {
        if (rrQueue.isEmpty()) {
            return null;
        }
        Task peekElem = rrQueue.pollFirst();
        int curBurst = remaindMap.get(peekElem.getTid());
        if (curBurst <= TIMEQUANTUM) {
            remaindMap.remove(peekElem.getTid());
        } else {
            remaindMap.put(peekElem.getTid(),curBurst - TIMEQUANTUM);
            rrQueue.addLast(peekElem);
        }
        return peekElem;
    }
}