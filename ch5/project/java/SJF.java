import java.util.*;

/**
 * SJF,Shortest-Job-First
 */
public class SJF implements Algorithm {
    private List<Task> sjfQueue = null;
    int nextIdx;
    public SJF(List<Task> queue) {
        sjfQueue = new ArrayList<>();
        sjfQueue.addAll(queue);
        nextIdx = 0;
        Collections.sort(sjfQueue,(t1,t2) -> {
            return t1.getBurst() - t2.getBurst();
        });
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
        if (nextIdx >= sjfQueue.size()) {
            return null;
        }
        nextIdx++;
        return sjfQueue.get(nextIdx);
    }
}