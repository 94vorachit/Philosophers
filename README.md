*This Project Has Been Created As Part Of The 42 Curriculum By **Vorhansa***

# Description

The **Philosophers** project is a 42 School project designed to introduce the fundamentals of **multithreading, mutexes, synchronization, and concurrent programming in C**.

The project is based on the classic **Dining Philosophers Problem**. A number of philosophers sit around a table, and each philosopher needs two forks to eat. Philosophers alternate between eating, sleeping, and thinking.

The challenge is to create a program where all philosophers can operate concurrently while avoiding:

* **Deadlocks**
* **Data races**
* **Starvation**
* Incorrect timing and death detection
* Unsafe access to shared resources

Each philosopher is represented by a thread, while each fork is protected by a mutex. A monitoring system continuously checks whether a philosopher has died from not eating within the required time or whether all philosophers have completed the required number of meals.

## Mandatory
The subject describes the problem as follows:
* One or more philosophers sit at a round table.
* There is a large bowl of spaghetti in the middle of the table.
* The spaghetti can only be eaten with two forks.
* There are only as many forks as there are philosophers.
* Each philosopher successively eats, sleeps and thinks.
* If a philosopher hasn't eaten in a certain timeframe, he will die of starvation.
* Philosophers cannot communicate with each other.

# Instructions

## Compilation

The project includes a `Makefile` with the following targets:

```bash
make
make clean
make fclean
make re
```

To compile the project:

```bash
make
```

This creates the executable:

```bash
./philo
```

## Execution

The program accepts the following arguments:

```text
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Arguments

| Argument                                    | Description                                                   |
| ------------------------------------------- | ------------------------------------------------------------- |
| `number_of_philosophers`                    | Number of philosophers and forks                              |
| `time_to_die`                               | Time in milliseconds a philosopher can survive without eating |
| `time_to_eat`                               | Time in milliseconds spent eating                             |
| `time_to_sleep`                             | Time in milliseconds spent sleeping                           |
| `number_of_times_each_philosopher_must_eat` | Optional number of meals each philosopher must complete       |

## Example

```bash
./philo 5 800 200 200
```

This starts 5 philosophers with:

* 800 ms before a philosopher dies
* 200 ms eating time
* 200 ms sleeping time

An example with a meal limit:

```bash
./philo 5 800 200 200 3
```

The program terminates once every philosopher has eaten at least 3 times.

## Output

The program prints the actions of each philosopher in the following format:

```text
timestamp_in_ms philosopher_id has taken a fork
timestamp_in_ms philosopher_id is eating
timestamp_in_ms philosopher_id is sleeping
timestamp_in_ms philosopher_id is thinking
timestamp_in_ms philosopher_id died
```

The timestamp represents the elapsed time since the simulation started.

## Technical Choices

### Threads

Each philosopher is represented by a `pthread` thread. This allows all philosophers to perform their routines concurrently.

<p align="center">
  <a href="https://www.geeksforgeeks.org/c/multithreading-in-c/"><img src="https://media.geeksforgeeks.org/wp-content/uploads/20260409105919761667/frame_1.webp" alt="thread" ></a>
</p>

### Mutexes

Each fork is protected by a mutex. A philosopher must lock the appropriate fork mutex before using a fork and unlock it after eating.

<p align="center">
  <a href="https://www.geeksforgeeks.org/cpp/std-mutex-in-cpp/"><img src="https://media.geeksforgeeks.org/wp-content/uploads/20231111130347/mutex-locks.webp" alt="Working of Mutex in C" ></a>
</p>

Additional mutexes are used to protect shared state, such as:

* Death status
* Philosopher state
* Output to the terminal
* Meal information

## Deadlock Prevention

To prevent deadlocks, philosophers do not all attempt to acquire their forks in the same order.

The implementation uses a consistent fork-ordering strategy so that philosophers cannot create a circular wait condition.

## Monitoring

A separate monitoring process checks the philosophers regularly.

The monitor checks whether:

1. A philosopher has exceeded `time_to_die`.
2. All philosophers have completed the required number of meals.

When a philosopher dies, the simulation stops and the death is printed.

## Timing

The project uses `gettimeofday()` to obtain the current time and calculate timestamps in milliseconds.

A custom sleep function is used instead of relying solely on `usleep()`, allowing the program to repeatedly check the simulation state while waiting.

# Resources
> 42 School Philosophers project designed around Edsger Dijkstra's famous [dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem).

* [thread](https://www.geeksforgeeks.org/c/multithreading-in-c/)

* [Mutex](https://www.geeksforgeeks.org/cpp/std-mutex-in-cpp)

### tester
https://github.com/dantonik/42-philosophers-tester.git

# Additional

## AI Usage
GitHub Copilot (GPT-5 mini backend) is used for
* Is used to analyze other's people code sample, comprehend the code logic.
* Is used to help with debugging where logic errors occured.
* Discussed about logic
* Used for finding information about micro-optimization, techniques, coding convention.

## Functions Usage
* `pthread_create()` — POSIX thread creation.
* `pthread_join()` — waiting for a thread to finish.
* `pthread_mutex_init()` — mutex initialization.
* `pthread_mutex_lock()` / `pthread_mutex_unlock()` — mutex synchronization.
* `gettimeofday()` — obtaining the current time.
* `usleep()` — short-duration sleeping.

## Documentation

In C, threads allow a program to run multiple execution paths concurrently within the same shared memory space, while mutexes (mutual exclusion locks) prevent race conditions by ensuring only one thread accesses shared data at a time. On Unix-like systems, both are typically implemented using the POSIX Threads library (<pthread.h>), though modern C (C11 and later) also offers native <threads.h>.

## Core Functions (<pthread.h>)

To use POSIX threads and mutexes, include <pthread.h> and link the library during compilation using the -pthread flag (e.g., gcc main.c -pthread).
• Thread Creation & Management.
	• `pthread_create()` spawns a new thread and starts executing a specified function.
	• `pthread_join()` blocks the calling thread until the target thread finishes execution.
	• `pthread_detach()` marks a thread so its resources are automatically reclaimed upon termination without needing a join.
• Mutex Operations.
	• `pthread_mutex_init()` initializes a mutex dynamically (or use PTHREAD_MUTEX_INITIALIZER for static initialization).
	• `pthread_mutex_lock()` acquires the lock, blocking the thread if another thread currently holds it.
	• `pthread_mutex_unlock()` releases the lock so waiting threads can proceed.
	• `pthread_mutex_destroy()` frees resources allocated to the mutex when it is no longer needed.


## Common Pitfalls & Best Practices

• Deadlocks. A deadlock occurs when two or more threads wait indefinitely for locks held by each other. Always lock multiple mutexes in the exact same order across all threads, and never lock a standard mutex twice in the same thread.
• Keep Critical Sections Short. Only lock code that directly reads or writes shared data. Performing slow I/O or heavy computations inside a lock forces other threads to wait unnecessarily and destroys concurrency performance.
• Debugging Tools. Race conditions can be intermittent and hard to reproduce. Compile your code with ThreadSanitizer `(gcc -fsanitize=thread -g main.c -pthread)` or use Valgrind's Helgrind tool `(valgrind --tool=helgrind ./a.out)` to detect data races and lock-order violations automatically.

For deeper details on POSIX threading primitives, refer to the codequoi guide on Threads and Mutexes or the GeeksforGeeks C Multithreading overview.

Would you like to explore one of these next steps to deepen your understanding?
• Condition Variables (pthread_cond_t): Learn how to make threads sleep and wake up efficiently based on specific state changes (such as building a producer-consumer queue).
• Standard C11 <threads.h> vs. POSIX <pthread.h>: Compare portable native C threading syntax with traditional POSIX functions.
• Deadlock Prevention & Try-Locks: See how to use pthread_mutex_trylock() and lock hierarchies to handle complex multi-lock scenarios safely.

## Multithreading in C

Multithreading is a technique in which a process is divided into multiple threads that execute concurrently. It allows a program to perform multiple tasks simultaneously within a single process, improving performance and responsiveness.

* Threads are lightweight execution units within a process.
* Multiple threads share resources while executing independently.

#### Example: The following program demonstrates the creation and execution of a thread using the POSIX Threads (pthreads) library.

```c
#include <pthread.h>
#include <stdio.h>

void* foo(void* arg)
{
    printf("Thread is running\n");
    return NULL;
}

int main()
{
    pthread_t thread;

    pthread_create(&thread, NULL, foo, NULL);
    pthread_join(thread, NULL);

    return 0;
}
```
#### Output
```
Thread is running
```
#### Explanation: The program creates a new thread using pthread_create(). The main thread waits for the created thread to finish using pthread_join().

## Threads
A thread represents an independent sequence of execution within a process. Unlike processes, threads share the same memory space and operating system resources.

* Each `thread` has its own program counter, registers, and stack.
* Threads share code, data, files, and signals.
* Thread creation is faster than process creation.
* Multiple threads can execute concurrently.

### Implementing Multithreading Using Pthreads
The POSIX Threads (pthread) library provides APIs for creating, managing, synchronizing, and terminating threads in C. To use pthreads, include the header file:
```c
#include <pthread.h>
```
If required, compile the program using:
```
gcc program.c -lpthread
```
### Creating a Thread
The pthread_create() function creates and starts a new thread.

#### Syntax
```c
pthread_create(thread, attr, routine, arg);
```
#### Parameters

* thread - Stores the ID of the newly created thread.
* attr - Specifies thread attributes (NULL for default).
* routine - Function executed by the thread.
* arg - Argument passed to the thread function.
```c
#include <pthread.h>
#include <stdio.h>

void* foo(void* arg) {
    printf("Created a new thread");
    return NULL;
 }
int main() {
    
    // Create a pthread_t variable to store
    // thread ID
    pthread_t thread1;
    
    // Creating a new thread. 
    pthread_create(&thread1, NULL, foo, NULL);
    return 0;
}
```
#### output
```
Created a new thread
```

#### Explanation: pthread_create() creates a thread that executes the foo() function.

### Waiting for a Thread to Finish
The pthread_join() function blocks the calling thread until the specified thread terminates.
```c
#include <pthread.h>
#include <stdio.h>

void* foo(void* arg) {
    printf("Thread is running.\n");
    return NULL;
}

int main() {
    pthread_t thread1;
    pthread_create(&thread1, NULL, foo, NULL);

    // Wait for thread to finish
    pthread_join(thread1, NULL);

    return 0;
}
```

#### output
```
Thread is running
```

### Explicitly Terminating a Thread
The pthread_exit() function terminates the calling thread explicitly.

#### Syntax
```
pthread_exit(value);
```

```c
void* foo(void* arg)
{
    printf("Thread is running.\n");

    pthread_exit(NULL);

    printf("This statement never executes.\n");
}
```

#### output
```
Thread is running.
```

### Cancelling a Thread
The `pthread_cancel()` function is used to request the cancellation of a thread. It sends a cancellation request to the target thread, but the actual termination depends on whether the thread is in a cancellable state and if it handles cancellation. Example:
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* myThreadFunc(void* arg) {
    while(1) {
        printf("Thread is running...\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, myThreadFunc, NULL);
    sleep(5);
    //Requesting to cancel the thread after 5 seconds. 
    pthread_cancel(thread);
    // Wait for the thread to terminate
    pthread_join(thread, NULL);  

    printf("Main thread finished.\n");
    return 0;
}
```

#### output
```
Thread is running...
Thread is running...
Thread is running...
Thread is running...
Thread is running...
Thread is running...
Main thread finished.
```

### Getting the Current Thread ID
`pthread_self()` function returns the thread ID of the calling thread. This is useful when you need to identify or store the ID of the current thread in multi-threaded programs. Example
```c
#include <pthread.h>
#include <stdio.h>

void* foo(void* arg) {
    
    // Get current thread ID
    pthread_t thisThread = pthread_self();
    printf("Current thread ID: %lu\n",
        (unsigned long)thisThread);
    return NULL;
}

int main() {
    pthread_t thread1;
    pthread_create(&thread1, NULL, foo, NULL);
    pthread_join(thread1, NULL);
    return 0;
}
```

#### output
```
Current thread ID: 134681321096896
```

### Common Multithreading Issues
Multithreading improves performance but also introduces several challenges.

* Race Conditions: A race condition occurs when multiple threads access shared data simultaneously, producing unpredictable results.
    * Example: Two threads updating the same counter variable.

* Deadlocks: A deadlock occurs when multiple threads wait indefinitely for resources held by each other.
    * Example: Thread A waits for a resource held by Thread B, while Thread B waits for a resource held by Thread A.

* Starvation: Starvation occurs when a thread is continuously denied access to resources.
    * Example: A low-priority thread waiting indefinitely while high-priority threads continue executing.

### Thread Synchronization
Thread synchronization ensures safe access to shared resources. Common synchronization mechanisms include:

* Mutexes - Allow only one thread to access a resource at a time.
* Semaphores - Control access using counters.
* Condition Variables - Allow threads to wait for specific conditions.
* Barriers - Synchronize multiple threads at predefined points.
* Read-Write Locks - Permit multiple readers and a single writer.

### Advantages of Multithreading
Multithreading offers several benefits that improve the efficiency and responsiveness of modern applications:

* Improves overall application performance and enhances CPU utilization.
* Provides better responsiveness and improves the user experience.
* Enables efficient sharing and utilization of system resources.
* Improves scalability when handling multiple concurrent tasks.

### Disadvantages of Multithreading
Despite its advantages, multithreading also introduces several challenges:

* Increases the complexity of program design and implementation.
* Requires careful synchronization to manage shared resources safely.
* Introduces concurrency-related issues such as race conditions and deadlocks.
* Adds additional overhead for thread creation, scheduling, and management.