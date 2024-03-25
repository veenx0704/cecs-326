// Montserrat Alonso
// Taiki Tsukahara
// CECS-326 Operating Systems
// Group Project 2: Synchronization
// Due: 3-15-24

/* Dining Philosopher's problem:
 *	5 philosophers are engaged in Thinking and Eating
 *	5 Plate and 5 Forks:
 *		Every philosopher needs 2 forks to eat
 *
 */
#include <iostream>
#include <pthread.h>  //to use pthread_mutex_t,pthread_cond+t
#include <unistd.h>
using namespace std;

// #define is used to create constants:

// Number of philosophers:
#define NUM 5
// Philosopher is not eating and engaged in thinking with the state of 2:
#define THINKING 2
// Philosopher is unable to eat but is wanting to eat and are waiting with the
// state of 1:
#define HUNGRY 1
//Philospher is eating with the state of 0:
#define EATING 0
//LEFT determines index of the left fork, phnum is the index of the current phiolosopher, NUM is the total of philosophers:
#define LEFT (phnum + NUM - 1) % NUM      
//RIGHT determines index of the right fork, phnum is the index of the current phiolosopher, NUM is the total of philosophers:
#define RIGHT (phnum + 1) % NUM

//pthread_mutex_t is a data type used to declare a mutex variable
pthread_mutex_t mutex;       // Mutex for critical sections
                            //Mutex gives us 2 options: Locking(when a threads wants to acces a shared resource it locks the mutex and if the mutex is already locked by                             //another thread, the calling thread will wait until its available) and Unblocking(When a thread is done with the resource it unlocks the                                //mutex, so then other threads can access the resource)

pthread_cond_t cond_var;     // Condition variable for synchronization, works in conjuction with mutex

int state[NUM];              // Array to track the state of each philosopher

pthread_t philosophers[NUM]; // Array to store philosopher threads

// Function to make a philosopher pick up forks to eat
void pickup_forks(int phnum) {
    pthread_mutex_lock(&mutex); // Acquire the mutex lock because the mutex ensures one thread can execute the critical section at a time

    state[phnum] = HUNGRY; // Update philosopher state to hungry
    cout << "Philosopher " << phnum << " is Hungry" << endl;

    // Wait while neighboring(left or right) philosophers are eating
    while (state[LEFT] == EATING || state[RIGHT] == EATING)
        pthread_cond_wait(&cond_var, &mutex);

    // If forks are available, change the philosopher as eating
    state[phnum] = EATING;
    cout << "Philosopher " << phnum << " picks up forks " << LEFT << " and "
        << phnum << endl;
    cout << "Philosopher " << phnum << " is Eating" << endl;

    pthread_mutex_unlock(&mutex); // Release the mutex lock
}

// Function to make a philosopher return forks after eating
void return_forks(int phnum) {
    pthread_mutex_lock(&mutex); // Acquire the mutex lock

    state[phnum] = THINKING; // Update philosopher state to thinking
    cout << "Philosopher " << phnum << " puts down forks " << LEFT << " and "
        << phnum << endl;
    cout << "Philosopher " << phnum << " is Thinking" << endl;

    // Signal the condition variable to wake up waiting threads
    pthread_cond_signal(&cond_var);

    pthread_mutex_unlock(&mutex); // Release the mutex lock
}

// Function representing the behavior of each philosopher thread
void* philosopher(void* arg) {
    while (true) {
        int* phnum = (int*)arg;  // Cast argument to philosopher number
        sleep(1);                 // Simulate thinking time
        pickup_forks(*phnum);     // Attempt to eat
        sleep((unsigned int)0.5); // Simulate eating time
        return_forks(*phnum);     // Return forks and think again
    }
}

int main() {
    pthread_mutex_init(&mutex, NULL);   // Initialize mutex
    pthread_cond_init(&cond_var, NULL); // Initialize condition variable

    // Initialize philosopher states
    for (int i = 0; i < NUM; i++) {
        state[i] = THINKING;
        int* index =
            new int(i); // Create a new integer pointer for each philosopher number
        pthread_create(&philosophers[i], NULL, philosopher,
            (void*)index); // Create philosopher threads
    }

    // Join all philosopher threads after they finish execution
    for (int i = 0; i < NUM; i++) {
        pthread_join(philosophers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);   // Destroy mutex
    pthread_cond_destroy(&cond_var); // Destroy condition variable

    return 0;
}
