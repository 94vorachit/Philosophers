*This Project Has Been Created As Part Of The 42 Curriculum By **Vorhansa***
# Description

### Mandatory
The subject describes the problem as follows:
* One or more philosophers sit at a round table.
* There is a large bowl of spaghetti in the middle of the table.
* The spaghetti can only be eaten with two forks.
* There are only as many forks as there are philosophers.
* Each philosopher successively eats, sleeps and thinks.
* If a philosopher hasn't eaten in a certain timeframe, he will die of starvation.
* Philosophers cannot communicate with each other.

The program takes the following arguments:
```c
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```
* `number_of_philosophers`: the number of philosophers around the table.
* `time_to_die`: a number representing the time in milliseconds a philosopher has to live after a meal. If a philosopher hasn’t started eating time_to_die milliseconds after the beginning of his last meal or the beginning of the simulation, he will die.
* `time_to_eat`: a number representing the time in milliseconds a philosopher takes to finish his meal. During that time, the philosopher keeps his two forks in hand.
* `time_to_sleep`: the time in milliseconds that a philosopher spends sleeping.
* `number_of_times_each_philosopher_must_eat`: an optional argument that allows the program to stop if all the philosophers have eaten at least that many times. If this argument is not specified, the simulation carries on unless a philosopher dies.

Program logs philosophers actions in this format. X is philosopher ID:
```c
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
```
# Instructions

# Resources
https://github.com/dpetrosy/42-Philosophers/tree/master

# Additional

### AI Usage
GitHub Copilot (GPT-5 mini backend) is used for
* Is used to analyze other's people code sample, comprehend the code logic.
* Is used to help with debugging where logic errors occured.
* Discussed about logic
* Used for finding information about micro-optimization, techniques, coding convention.

### Functions Explained