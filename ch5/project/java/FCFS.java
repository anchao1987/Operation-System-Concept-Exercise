import java.util.*;
/**
 * FCFS,First Come First Serve
 */
public class FCFS implements Algorithm {
    private List<Task> queue;
    int nextIdx = 0;
    public FCFS(List<Task> queue) {
        this.queue = queue;
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
        if (nextIdx >= queue.size()) {
            return null;
        }
        nextIdx++;
        return queue.get(nextIdx);
    }
}