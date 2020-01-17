import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class DiningServerImpl implements DiningServer {

    Lock lock = new ReentrantLock();
    Condition[] conds;
    int philosopherCount;
    
    public DiningServerImpl(int philosopherCount) {
        this.philosopherCount = philosopherCount;
        for (int i = 0; i < philosopherCount; i++) {
            conds[i] = lock.newCondition();
        }
    }

    @Override
    public void takeForks(int philosopherNumber) {
        lock.tryLock();
        try {
            conds[(philosopherNumber+1) % philosopherCount].await();
            conds[(philosopherNumber + philosopherCount
                - 1) % philosopherCount].await();
        } catch(InterruptedException ire) {
            // Do Nothing
        } finally {
            lock.unlock();
        }
    }

    @Override
    public void returnForks(int philosopherNumber) {
        lock.tryLock();
        try {
            conds[(philosopherNumber+1) % philosopherCount].signal();
            conds[(philosopherNumber + philosopherCount
                - 1) % philosopherCount].signal();
        } 
        finally {
            lock.unlock();
        }
    }

}