#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // memset
#include <pthread.h>   // pthread_t, pthread_create, pthread_join
#include <semaphore.h> // sem_init, sem_wait, sem_post
#include <time.h>
#include <unistd.h>

#define SLEEP_MAX 20
/* the buffer */
buffer_item buffer[BUFFER_SIZE];

//sematphores
sem_t empty;
sem_t full;

//sem_t mutex;
pthread_mutex_t mutex;
int head,tail;
void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[])
{
    /* 1. Get command line arguments argv[1],argv[2],argv[3] */
    int waitTime = atoi(argv[1]);
    int producerNum = atoi(argv[2]);
    int consumerNum = atoi(argv[3]);

    //set random 
 	srand(time(NULL)); 
    /* 2. Initialize buffer */
    head = 0;
    tail = 0;
  	sem_init(&full,0,1);
    sem_init(&empty,0,1);
    pthread_mutex_init(&mutex,NULL);
  	memset(buffer, 0, BUFFER_SIZE);

    /* 3. Create producer thread(s) */
    pthread_t *producers;
    producers = (pthread_t*)malloc(sizeof(pthread_t) * producerNum);
    for (int i = 0; i < producerNum; i++) {
        producers[i] = pthread_create(&producers[i],NULL,producer,NULL);
    }
    /* 4. Create consumer thread(s) */
    pthread_t *consumers;
    consumers = (pthread_t*)malloc(sizeof(pthread_t) * consumerNum);
    for (int i = 0; i < consumerNum; i++) {
        consumers[i] = pthread_create(&consumers[i],NULL, consumer, NULL);
    }
    /* 5. Sleep */
    sleep(waitTime);
    /* 6. Exit */
    return 0;
}
int insert_item(buffer_item item)
{
    pthread_mutex_lock(&mutex);
    /* insert item into buffer */
    if ((tail + 1) % BUFFER_SIZE == head) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    buffer[(tail + 1) % BUFFER_SIZE] = item;
    tail = (tail+1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);

    return 0;
}

/* remove an object from buffer
    placing it in item
    return 0 if successful, otherwise
    return -1 indicating an error condition */
int remove_item(buffer_item *item)
{
    pthread_mutex_lock(&mutex);
    if (head == tail) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    *item = buffer[(head + 1) % BUFFER_SIZE];
    head = (head + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);
    return 0;
}



void *producer(void *param)
{
    buffer_item item;
    
    while (1)
    {
        /* sleep for a random period of time */
        int sleepTime = rand() % SLEEP_MAX + 1;
        sleep(sleepTime);
        /* generate a random number */
        item = rand();
        if (insert_item(item)) {
            printf("buffer is full\n");
            sem_wait(&empty);
        }
        else {
            printf("producer produced %d\n", item);
            sem_post(&full);
        }
    }
}
void *consumer(void *param)
{
    buffer_item item;
    while (1)
    {
        int sleepTime = rand() % SLEEP_MAX + 1;
        /* sleep for a random period of time */
        sleep(sleepTime);
        if (remove_item(&item)) {
            printf("buffer is empty\n");
            sem_wait(&full);
        }
        else {
            printf("consumer consumed %d\n", item);
            sem_post(&empty);
        }
    }
}