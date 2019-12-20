import java.util.*;
/**
 * Priority Schedule
 */
public class Priority implements Algorithm {
    private List<Task> priorityQueue = null;
    int nextIdx = 0;
    public Priority(List<Task> queue) {
        priorityQueue = new ArrayList<>();
        priorityQueue.addAll(queue);
        Collections.sort(priorityQueue,(t1,t2) -> {
            return t2.getPriority() - t1.getPriority();
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
        if (nextIdx >= priorityQueue.size()) {
            return null;
        }
        nextIdx++;
        return priorityQueue.get(nextIdx);
    }
}