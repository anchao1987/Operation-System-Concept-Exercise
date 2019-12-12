/**
 * Fork/join parallelism in Java
 *
 * Figure 4.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

import java.util.Arrays;
import java.util.concurrent.*;

public class SortTask extends RecursiveAction
{
    /**
     *
     */
    private static final long serialVersionUID = 1L;
    static final int SIZE = 1000;
    static final int THRESHOLD = 50;

    private int begin;
    private int end;
    private int[] array;

    public SortTask(int begin, int end, int[] array) {
        this.begin = begin;
        this.end = end;
        this.array = array;
    }

    protected void compute() {
        if (end - begin < 1) {
            return;
        }
        else if (end - begin < THRESHOLD) {
            // conquer stage 
            for (int i = begin + 1; i <= end; i++) {
                int key = array[i];
                int j = i - 1;
                while (j >= 0 && array[j] > key) {
                    array[j + 1] = array[j];
                    j--;
                }
                array[j + 1] = key;
            }
        

            return;
        }
        else {
            int pivoit = array[end];
            int i = begin;
            for (int j = begin; j < end; j++) {
                if (array[j] < pivoit) {
                    int temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                    i++;
                }
            }
            array[end] = array[i];
            array[i] = pivoit;
            
            SortTask leftTask = new SortTask(begin, i-1, array);
            SortTask rightTask = new SortTask(i + 1, end, array);

            leftTask.fork();
            rightTask.fork();
            leftTask.join();
            rightTask.join();
            return;
        }
    }

	public static void main(String[] args) {
		ForkJoinPool pool = new ForkJoinPool();
		int[] array = new int[SIZE];

		// create SIZE random integers between 0 and 9
		java.util.Random rand = new java.util.Random();

		for (int i = 0; i < SIZE; i++) {
			array[i] = rand.nextInt(1000);
		}		

		// use fork-join parallelism to sum the array
		SortTask task = new SortTask(0, SIZE-1, array);

        pool.invoke(task);
        
        for (int i = 1; i < SIZE; i++) {
            if (array[i] < array[i-1]) {
                System.out.println("No Sorting");
            }
        }
        System.out.println(Arrays.toString(array));

        System.out.println("BINGO, SORT");
	}
}

