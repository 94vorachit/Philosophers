Searched for regex `odd|even|if \(|while \(|pthread_mutex|fork|philo` (`**/philo/srcs/**`), 150 results

Read [](file:///home/vorhansa/Documents/Philosophers/philo/includes/philo.h)

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/simulation.c)

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/sim_action.c)

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/sim_utils2.c)

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/init.c)

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/init2.c)

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/sim_monitor.c#108-108), lines 108 to 176

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/sim_monitor.c#13-13), lines 13 to 108

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/sim_utils.c)

Read [](file:///home/vorhansa/Documents/Philosophers/philo/srcs/main.c#1-1), lines 1 to 120

## Project overview

This is a 42-style **Dining Philosophers** simulation.

- Every philosopher is a POSIX thread.
- Every fork is protected by a `pthread_mutex_t`.
- A monitor thread checks for starvation or completion.
- Shared data is protected with separate stop, state, and print mutexes.

The philosophers sit in a circular table:

```text
Philosopher 1 -> fork 0 and fork 1
Philosopher 2 -> fork 1 and fork 2
...
Last philosopher -> last fork and fork 0
```

## Initialization

`init_sim()`:

1. Records the simulation start time.
2. Allocates arrays for philosophers and forks.
3. Initializes shared mutexes.
4. Initializes every fork mutex.
5. Assigns IDs and left/right forks to every philosopher.

## Normal philosopher cycle

Each philosopher repeatedly follows:

```text
think -> take two forks -> eat -> release forks -> sleep -> think
```

In `philo_cycle()`:

- `lock_forks()` acquires both fork mutexes.
- `philo_eat()` updates:
  - `last_meal_ms`
  - `meals_eaten`
- `unlock_forks()` releases both forks.
- The philosopher then sleeps and starts thinking.

## Fork handling

Fork indexes are acquired in ascending order:

```text
lock the lower-index fork first
lock the higher-index fork second
```

This reduces deadlock risk, even though philosophers have different left and right fork references.

Stopping is also checked between lock operations. If the stop flag becomes true, the thread releases the mutex it acquired and exits.

## Even philosopher count

When the total count is even:

- The first action is a startup delay for **even-numbered philosophers**.
- Odd-numbered philosophers generally start immediately.
- This staggers one-half of the table from the other half.

The intended effect is:

- Odd philosophers begin acquiring forks.
- Even philosophers start eating slightly later.
- It reduces simultaneous first-turn conflicts.

## Odd philosopher count

When the total count is odd:

- The startup logic staggers **even philosophers**.
- In `philo_routine()`, odd IDs and some final IDs can receive extra startup delays.
- `lock_forks()` also adds a short 1 ms delay for even IDs.
- The `turn_lock` and `turn_id` logic exists for odd-table turn management, but the code does not clearly use `turn_id` to block fork acquisition.

Conceptually, odd counts are harder because there is no perfectly symmetrical split. The delays spread startup attempts over time, reducing three-way contention around the table.

The extra startup condition:

```c
if (odd_table && (even_id || id > threshold))
```

generally delays even philosophers and the final few philosophers.

## Special case: one philosopher

A table with one philosopher cannot possibly acquire two forks.

`single_philo_case()`:

1. Locks the only fork.
2. Prints that it took the fork.
3. Sleeps for `time_to_die`.
4. Unlocks the fork.
5. Sets `stop` and prints `died`.

## Monitor logic

The monitor runs independently and every 100 microseconds:

- Checks whether every philosopher reached `must_eat`.
- Checks whether:

```text
current_time - last_meal_ms > time_to_die
```

If every philosopher ate enough, it sets `stop = true`.

If a philosopher starves, the monitor:

1. Locks `print_lock`.
2. Locks `stop_lock`.
3. Sets `stop = true`.
4. Prints exactly one `died` message.

## Shared synchronization

| Mutex | Purpose |
|---|---|
| `fork_lock` | Protects each individual fork |
| `state_lock` | Protects meal counts and last-meal times |
| `stop_lock` | Protects the global simulation stop flag |
| `print_lock` | Prevents mixed or overlapping messages |
| `turn_lock` | Stores odd-table turn bookkeeping |

## End of simulation

The main thread:

1. Creates the monitor thread.
2. Creates all philosopher threads.
3. Joins all philosopher threads.
4. Joins the monitor thread.
5. Cleans up memory and destroys mutexes.

`custom_usleep()` ensures that sleeping, eating, and startup waits stop immediately when the monitor sets the stop flag.


# UPDATE

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/includes/philo.h)

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/srcs/init.c)

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/srcs/simulation.c)

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/srcs/sim_action.c)

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/srcs/sim_monitor.c)

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/srcs/sim_utils2.c)

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/srcs/sim_utils.c)

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/srcs/main.c)

Read [](file:///home/vorhansa/Documents/last%20check%20before%20finished/philo/philo/srcs/init2.c)

## Project overview

The project simulates the dining philosophers problem using one pthread per philosopher.

### Main flow

1. `main()` parses arguments and initializes the simulation.
2. One mutex represents each fork.
3. A philosopher thread repeatedly:
   - Takes two forks.
   - Eats and updates `last_meal_ms` and `meals_eaten`.
   - Releases both forks.
   - Sleeps.
   - Thinks.
4. A monitor thread continuously checks:
   - Whether a philosopher exceeded `time_to_die`.
   - Whether every philosopher reached `must_eat`.
5. When the simulation stops, every thread exits and all mutexes/memory are cleaned up.

### Shared synchronization

- `fork` mutexes: protect fork ownership.
- `state_lock`: protects meal counts and timestamps.
- `stop_lock`: protects the global stop flag.
- `print_lock`: prevents simultaneous/damaged console output.

### Fork acquisition logic

`lock_forks()` always acquires forks in ascending fork-index order:

- Prevents a circular waiting deadlock.
- For example, philosopher 0 takes fork 0 before fork 1.
- Philosopher 1 takes fork 1 before fork 2.
- The last philosopher also uses the circular fork 0.

After eating, `unlock_forks()` releases both left and right forks.

## Odd philosopher count

For an odd number of philosophers, the code adds a short stagger:

```c
if (philo_count % 2 == 1 && philo->id % 2 == 0)
	custom_usleep(1, sim);
```

Therefore, even-numbered philosophers pause briefly before fork acquisition.

The intended purpose is to reduce simultaneous fork contention. Odd tables do not have perfectly symmetric opposite groups, so this extra stagger helps prevent immediate deadlock and starvation.

## Even philosopher count

For an even number of philosophers:

- The special `1 ms` odd-table delay is not used.
- In `philo_routine()`, every even philosopher initially waits for `time_to_eat`.
- This initially separates the even and odd groups:
  - Odd philosophers start immediately.
  - Even philosophers wait.
  - They later alternate eating activity more evenly.

This gives the odd IDs a chance to acquire and release forks before the even IDs begin.

## One-philosopher case

A single philosopher can hold only one fork, never both:

1. Lock the only fork.
2. Print `has taken a fork`.
3. Wait for `time_to_die`.
4. Release the fork.
5. Print `<time> 1 died`.

There is no eating cycle in this case.

## Stopping logic

The simulation stops when:

- One philosopher exceeds `time_to_die`, or
- `must_eat` is provided and every philosopher reaches that meal count.

Once stopped, `custom_usleep()` exits early and philosopher cycles stop promptly.
