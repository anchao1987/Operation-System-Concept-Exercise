import java.util.*;

/**
 * Priority with Round-Robin Algorithm
 */
public class PriorityRR implements Algorithm {
    private List<Algorithm> algorithms;
    int i = 0;
    public PriorityRR(List<Task> queue) {
        List<Task> unique = new ArrayList<>();
        boolean uniqueFlag = true;
        List<Task> repeat = new ArrayList<>();
        for (int i = 1; i < queue.size(); i++) {
            if (queue.get(i).getPriority() != queue.get(i-1).getPriority()) {
                if (uniqueFlag) {
                    unique.add(queue.get(i-1));
                } else {
                    repeat.add(queue.get(i-1));
                    Algorithm rr = new RR(repeat);
                    algorithms.add(rr);
                    repeat = new ArrayList<>();
                    unique = new ArrayList<>();
                    uniqueFlag = true;
                }
            } else {
                if (uniqueFlag) {
                    repeat = new ArrayList<>();
                    repeat.add(queue.get(i-1));
                    Algorithm fcfs = new FCFS(unique);
                    algorithms.add(fcfs);
                    unique = new ArrayList<>();
                    uniqueFlag = false;
                } else {
                    repeat.add(queue.get(i-1));
                }
            }
        }
        if (uniqueFlag) {
            unique.add(queue.get(queue.size()-1));
            Algorithm fcfs = new FCFS(unique);
            algorithms.add(fcfs);
        } else {
            repeat.add(queue.get(queue.size() - 1));
            Algorithm rr  = new RR(repeat);
            algorithms.add(rr);
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
    
        Task next = algorithms.get(i).pickNextTask();
        if (next != null) {
            return next;
        } else if (i < algorithms.size()) {
            i++;
            next = algorithms.get(i).pickNextTask();
            return next;
        } else {
            return null;
        }
      
    }
}