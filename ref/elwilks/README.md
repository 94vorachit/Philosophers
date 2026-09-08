*This project has been created as part of the 42 curriculum by <elwilks>.*

# Philosophers

## Description

The **Philosophers** project is a 42 School project designed to introduce the fundamentals of **multithreading, mutexes, synchronization, and concurrent programming in C**.

The project is based on the classic **Dining Philosophers Problem**. A number of philosophers sit around a table, and each philosopher needs two forks to eat. Philosophers alternate between eating, sleeping, and thinking.

The challenge is to create a program where all philosophers can operate concurrently while avoiding:

* **Deadlocks**
* **Data races**
* **Starvation**
* Incorrect timing and death detection
* Unsafe access to shared resources

Each philosopher is represented by a thread, while each fork is protected by a mutex. A monitoring system continuously checks whether a philosopher has died from not eating within the required time or whether all philosophers have completed the required number of meals.

### Project Goal

The main goal of the project is to understand how multiple threads interact with shared data and how **mutexes can be used to safely synchronize access to shared resources**.

The project also requires accurate time management using functions such as `gettimeofday()` and careful handling of thread creation, joining, and synchronization.

## Instructions

### Compilation

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

### Execution

The program accepts the following arguments:

```text
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

#### Arguments

| Argument                                    | Description                                                   |
| ------------------------------------------- | ------------------------------------------------------------- |
| `number_of_philosophers`                    | Number of philosophers and forks                              |
| `time_to_die`                               | Time in milliseconds a philosopher can survive without eating |
| `time_to_eat`                               | Time in milliseconds spent eating                             |
| `time_to_sleep`                             | Time in milliseconds spent sleeping                           |
| `number_of_times_each_philosopher_must_eat` | Optional number of meals each philosopher must complete       |

### Example

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

### Output

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

### Mutexes

Each fork is protected by a mutex. A philosopher must lock the appropriate fork mutex before using a fork and unlock it after eating.

Additional mutexes are used to protect shared state, such as:

* Death status
* Philosopher state
* Output to the terminal
* Meal information

### Deadlock Prevention

To prevent deadlocks, philosophers do not all attempt to acquire their forks in the same order.

The implementation uses a consistent fork-ordering strategy so that philosophers cannot create a circular wait condition.

### Monitoring

A separate monitoring process checks the philosophers regularly.

The monitor checks whether:

1. A philosopher has exceeded `time_to_die`.
2. All philosophers have completed the required number of meals.

When a philosopher dies, the simulation stops and the death is printed.

### Timing

The project uses `gettimeofday()` to obtain the current time and calculate timestamps in milliseconds.

A custom sleep function is used instead of relying solely on `usleep()`, allowing the program to repeatedly check the simulation state while waiting.

## Resources

### Documentation

* `pthread_create()` — POSIX thread creation.
* `pthread_join()` — waiting for a thread to finish.
* `pthread_mutex_init()` — mutex initialization.
* `pthread_mutex_lock()` / `pthread_mutex_unlock()` — mutex synchronization.
* `gettimeofday()` — obtaining the current time.
* `usleep()` — short-duration sleeping.

Useful system documentation can be accessed through the Linux manual pages:

```bash
man pthread_create
man pthread_join
man pthread_mutex_lock
man pthread_mutex_unlock
man gettimeofday
man usleep
```

### Classic References

* **The Dining Philosophers Problem** — Edsger W. Dijkstra's classic concurrency problem.
* **POSIX Threads Programming** — documentation and tutorials covering threads and synchronization.
* **Linux `pthread` documentation** — references for POSIX thread and mutex functions.
* **The 42 Philosophers subject PDF** — the official project requirements and rules.

### AI Usage

AI was used as a learning and debugging aid during the development of this project.

It was mainly used to:

* Explain C concepts related to **pointers, structures, arrays, and dynamic memory allocation**.
* Explain how **pthreads and mutexes** work.
* Help understand **race conditions and deadlocks**.
* Analyse debugging output and help identify possible causes of incorrect philosopher behaviour.
* Explain timing issues involving `gettimeofday()`, milliseconds, `usleep()`, and custom sleep functions.
* Help analyse compiler and linker errors.
* Review parts of the implementation and suggest areas to investigate when debugging.

The project code was written and developed by the student. AI was used primarily to **explain concepts, analyse problems, and assist with debugging**, rather than as a replacement for understanding or implementing the project.

## Project Structure

The project is divided into several source files, each responsible for a different part of the program:

```text
philo/
├── Makefile
├── philo.h
├── main.c
├── init.c
├── monitor.c
├── parse.c
├── setup.c
├── routine.c
├── check.c
└── utils.c
```

The main responsibilities include:

* **Parsing and validation** of program arguments
* **Initialising** philosophers, forks, mutexes, and configuration
* **Creating and joining threads**
* **Running philosopher routines**
* **Managing fork access**
* **Monitoring philosopher deaths**
* **Checking meal completion**
* **Handling timing and utility functions**
* **Synchronising shared data**

## Learning Objectives

Through this project, I developed a better understanding of:

* POSIX threads
* Mutexes and synchronization
* Concurrent programming
* Race conditions
* Deadlock prevention
* Thread lifecycle management
* Shared memory
* Dynamic memory allocation
* Time measurement in C
* Debugging multithreaded programs
* Designing safe access to shared resources
