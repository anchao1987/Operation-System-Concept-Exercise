7.1 Explain why Windows and Linux implement multiple locking mechanisms.Describe the circumstances under which they use spinlocks,mutex locks, semaphores, and condition variables. In each case, explain why the mechanism is needed.

**Answer**

These operating systems provide different locking mechanisms depending on the application developers’ needs. Spinlocks are useful for multiprocessor systems where a thread can run in a busy-loop (for a short period of time) rather than incurring the overhead of being put in a sleep queue. Mutexes are useful for locking resources.  Semaphores and condition variables are more appropriate tools for synchronization when a resource must be held for a long period of time, since spinning is inefficient for a long duration.

7.2 Windows provides a lightweight synchronization tool called slim reader–writer locks. Whereas most implementations of reader–writer locks favor either readers or writers, or perhaps order waiting threads using a FIFO policy, slim reader–writer locks favor neither readers nor writers, nor are waiting threads ordered in a FIFO queue. Explain the benefits of providing such a synchronization tool.

**Answer**

Simplicity. IF RWlocks provide fairness or favor readers or writers, there is more overhead to the lock. By providing such a simple synchronization mechanism, access to the lock is fast. Usage of this lock may be most appropriate for situations where reader–locks are needed, but quickly acquiring and releasing the lock is similarly important.

7.3 Describe what changes would be necessary to the producer and consumer processes in Figure 7.1 and Figure 7.2 so that a mutex lock could be used instead of a binary semaphore.

**Answer**

7.4 Describe how deadlock is possible with the dining-philosophers problem.

**Answer**

7.5 Explain the difference between signaled and non-signaled states with Windows dispatcher objects.

**Answer**

An object in a signaled state is available, and a thread will not block when acquiring the object. An object in a nonsignaled state is not available, and a thread will block when attempting to acquire the object.

7.6 Assume val is an atomic integer in a Linux system.What is the value of val after the following operations have been completed?

```c
atomic_set(&val,10);
atomic_sub(8,&val);
atomic_inc(&val);
atomic_inc(&val);
atomic_add(6,&val);
atomic_sub(3,&val);
```

**Answer**

7

7.7 Describe two kernel data structures in which race conditions are possible. Be sure to include a description of how a race condition can occur.

**Answer**

There are many answers to this question. Some kernel data structures include a process id (pid) management system, kernel process table, and scheduling queues. With a pid management system, it is possible two processes may be created at the same time and there is a race condition assigning each process a unique pid. The same type of race condition can occur in the kernel process table: two processes are created at the same time and there is a race assigning them a location in the kernel process table. With scheduling queues, it is possible one process has been waiting for IO which is now available. Another process is being context-switched out. These two processes are being moved to the Runnable queue at the
same time. Hence there is a race condition in the Runnable queue.

7.8 The Linux kernel has a policy that a process cannot hold a spinlock while attempting to acquire a semaphore. Explain why this policy is in place.

**Answer**

Because acquiring a semaphore may put the process to sleep while it is waiting for the semaphore to become available. Spinlocks are to only be held for short durations and a process that is sleeping may hold the spinlock for too long a period.

7.9 Design an algorithm for a bounded-buffer monitor in which the buffers (portions) are embedded within the monitor itself.

**Answer**

```c
Monitor boundedbuffer{
	Const bufsize = 100;
	item buf[100];
	int in, out, count;
	condition Producer, Consumer;
	void produce (item x) {
		if (count == bufsize)
        	Producer.wait();
        buf[in] = x;
        count++;
        in = (in + 1) % n;
        Consumer.signal();
   }
   void consume (item& x) {
        if (count == 0)
            Consumer.wait();
        x = buf[out];
        count --;
        out = (out + 1) % n;
        Producer.signal();
	}
	initialization_code() {
		in =0;
		out =0;
        count =0;
    }
}
```

7.10 The strict mutual exclusion within a monitor makes the bounded-buffer monitor of Exercise 7.9 mainly suitable for small portions.
a. Explain why this is true.
b. Design a new scheme that is suitable for larger portions.

**Answer**

The solution to the bounded buffer problem given above copies the produced value into the monitor’s local buffer and copies it back from the monitor’s local buffer to the consumer. These copy operations could
be expensive if one were using large extents of memory for each buffer region. The increased cost of copy operation means that the monitor is held for a longer period of time while a process is in the produce or
consume operation. This decreases the overall throughput of the system. This problem could be alleviated by storing pointers to buffer regions within the monitor instead of storing the buffer regions themselves.
Consequently, one could modify the code given above to simply copy the pointer to the buffer region into and out of the monitor’s state. This operation should be relatively inexpensive and therefore the period
of time that the monitor is being held will be much shorter, thereby increasing the throughput of the monitor.

7.11 Discuss the tradeoff between fairness and throughput of operations in the readers–writers problem. Propose a method for solving the readers–writers problem without causing starvation.

**Answer**

Throughput in the readers-writers problem is increased by favoring multiple readers as opposed to allowing a single writer to exclusively access the shared values. On the other hand, favoring readers could
result in starvation for writers. The starvation in the readers-writers problem could be avoided by keeping timestamps associated with waiting processes. When a writer is finished with its task, it would wake
up the process that has been waiting for the longest duration. When a reader arrives and notices that another reader is accessing the database, then it would enter the critical section only if there are no waiting writers. These restrictions would guarantee fairness

7.12 Explain why the call to the lock() method in a Java ReentrantLock is not placed in the try clause for exception handling, yet the call to the unlock() method is placed in a finally clause.

**Answer**

lock() does not throw any checked exceptions. Consider what happens if we place lock() within the try clause and an unchecked exception occurs when lock() is invoked (such as OutofMemoryError): The
finally clause triggers the call to unlock(),which then throws the unchecked IllegalMonitorStateException, as the lock was never acquired. This IllegalMonitorStateException replaces the unchecked exception that occurred when lock() was invoked, thereby obscuring the reason why the program initially failed.

7.13 Explain the difference between software and hardware transactional memory.

**Answer**

HTM requires no special code instrumentation and thus has less overhead than STM. However, HTM
does require that existing cache hierarchies and cache coherency protocols be modified to support transactional memory.