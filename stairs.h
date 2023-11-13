#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>



typedef struct thread_arg {
    int index;
    int direction;
    double turnaround_time;
} thread_arg_t;

// global constants
// define any global constants you want to use in your code
 
// #define the maximum number of customers/threads in the system to test
// #define how many customer can be on the stairs at the same time
// you can also define other constants for your "prevent deadlock" or "prevent starvation" algorithm

// define timing variables



// thread
pthread_t *tid;


// Define global variables on the allowed direction, waiting threads, ...
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;

sem_t stair_direction_sem;    // Semaphore to ensure the stair is only for 1 direction
sem_t up_sem;       // Semaphore to ensure only 1 customer updating downCount
sem_t down_sem;     // Semaphore to ensure only 1 customer updating upCount
sem_t stair_sem; // allow max steps standing customer


// int onStairs = 0;
// int upOnStair = 0;
int numUp = 0; 
int numDown = 0; 
// int downOnStair = 0; 
int max_customers = 30;
int max_stairs = 13;

// write any helper functions you need here
