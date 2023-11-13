#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stairs.h>


void simulateCrossingTime() {
    sleep(1); // 1.5 seconds
}

void *threadfunction(void *vargp) {
    thread_arg_t *arg = (thread_arg_t *)vargp;

    clock_t start_time = clock();
    if (arg->direction == 0) {
 
        // first people wait the go up
    
        printf("customer %d waiting to go up\n", arg->index);
        sem_wait(&up_sem);

        numUp += 1; 
        if(numUp == 1){
            sem_wait(&stair_direction_sem);
        }

        if (sem_trywait(&stair_sem) != 0){
            printf("customer %d is waiting for a position to go up since stair has no space\n", arg->index);
            sem_wait(&stair_sem);
        }
        
        sem_post(&up_sem);
        
        printf("customer %d going up\n", arg->index);


        // 模拟上楼梯的时间
        simulateCrossingTime();   
        printf("customer %d finished going up\n", arg->index);
        numUp -= 1;
        sem_post(&stair_sem);   

        if (numUp == 0){
            sem_post(&stair_direction_sem);
            printf("stair direction changes to down\n");
        }
        
    } else {

        // first people wait the go down
        printf("customer %d waiting to go down\n", arg->index);
        sem_wait(&down_sem);
        
        numDown += 1; 
        if(numDown == 1){
            sem_wait(&stair_direction_sem);
        }

        if (sem_trywait(&stair_sem) != 0){
            printf("customer %d waiting for a position to go down since stair has no space\n", arg->index);
            sem_wait(&stair_sem);
        }
        
        // printf("customer %d got the position and starts going down\n", arg->index);
        sem_post(&down_sem);
        

        printf("customer %d going down\n", arg->index);

        // 模拟上楼梯的时间
        simulateCrossingTime();

        printf("customer %d finished going down\n", arg->index);
        numDown -= 1;
        sem_post(&stair_sem);

        if (numDown == 0){
            sem_post(&stair_direction_sem);
            printf("Crossing direction reset\n");
        }
    }

    clock_t end_time = clock();
    arg->turnaround_time = ((double)(end_time - start_time) * 1000000 / CLOCKS_PER_SEC);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_customers> <number_of_stairs>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numCustomers = atoi(argv[1]);
    int numStairs = atoi(argv[2]);

    if (numCustomers > max_customers || numStairs > max_stairs) {
        fprintf(stderr, "Number of customers or stairs exceeds maximum allowed\n");
        exit(EXIT_FAILURE);
    }

    printf("Number of Customers: %d\nNumber of stairs: %d\n", numCustomers, numStairs);

    // Initialize semaphore
    sem_init(&stair_sem, 0, numStairs);
    sem_init(&up_sem, 0, 1);
    sem_init(&down_sem, 0, 1);
    sem_init(&stair_direction_sem, 0, 1);

    // Allocate memory for thread IDs
    tid = (pthread_t *)malloc(sizeof(pthread_t) * numCustomers);

    // create an array to store arg
    thread_arg_t* args = (thread_arg_t *)malloc(sizeof(thread_arg_t)*numCustomers);

    srand(0xFF);
    // Create customer threads
    for (int i = 0; i < numCustomers; i++) {
        // thread_arg_t *arg = (thread_arg_t *)malloc(sizeof(thread_arg_t));
        args[i].index = i;
        args[i].direction = rand() % 2; // 0 for up, 1 for down
        pthread_create(&tid[i], NULL, threadfunction, &args[i]);
    }

    // Join customer threads
    for (int i = 0; i < numCustomers; i++) {
        pthread_join(tid[i], NULL);
    }

    // average turnaround time
    for (int i = 0; i < numCustomers; i ++){
        printf("Turnaround time for index %d = %f\n", i, args[i].turnaround_time);
    }

    // Free allocated memory
    free(tid);
    free(args);

    return 0;
}
