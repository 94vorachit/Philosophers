/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_action.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 19:02:07 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/17 14:56:48 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Purpose: Prints a philosopher action such as:
 * * `“has taken a fork”`
 * * `“is eating”`
 * * `“is sleeping”`
 * * `“is thinking”`
 * It does this safely without printing after the simulation is already stopped.
 * 
 * How it works:
 * 1. It first checks `sim->stop` under the `stop_lock` mutex.
 * 2. Then it locks `print_lock` to avoid mixed output from multiple threads.
 * 3. It double-checks `stop` again, just in case 
 * the simulation ended while waiting.
 * 4. If not stopped, it calculates:
 * * `elapsed = now_ms() - start_time_ms`
 * 
 * Then it prints:
 * 1. time in ms
 * 2. philosopher id
 * 3. message
 * 
 * This prevents race conditions and ensures clean console output.
 */
void	log_action(t_philo *philo, const char *msg)
{
	long long	elapsed;
	bool		stopped;

	pthread_mutex_lock(&philo->sim->stop_lock);
	stopped = philo->sim->stop;
	pthread_mutex_unlock(&philo->sim->stop_lock);
	if (stopped)
		return ;
	pthread_mutex_lock(&philo->sim->print_lock);
	pthread_mutex_lock(&philo->sim->stop_lock);
	stopped = philo->sim->stop;
	pthread_mutex_unlock(&philo->sim->stop_lock);
	if (!stopped)
	{
		elapsed = now_ms() - philo->sim->start_time_ms;
		printf("%lld %d %s\n", elapsed, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->sim->print_lock);
}

/**
 * Purpose: Updates the philosopher’s eating state and simulates eating.
 * 
 * How it works:
 * 1. It `locks state_lock` because it modifies shared state:
 * * `last_meal_ms = now_ms()`
 * * `meals_eaten += 1`
 * 2. Then it unlocks the mutex.
 * 3. Calls `log_action(philo, "is eating")`
 * 4. Calls `custom_usleep(philo->sim->time_to_eat, philo->sim)`
 * 
 * This means:
 * 1. the philosopher records when it last ate,
 * 2. counts meals,
 * 3. then spends the required time eating.
 * 
 * The `state_lock` is important because the monitor thread also 
 * reads these values to detect death and completion.
 */
void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->state_lock);
	philo->last_meal_ms = now_ms();
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->sim->state_lock);
	log_action(philo, "is eating");
	custom_usleep(philo->sim->time_to_eat, philo->sim);
}

/**
 * Purpose: Handles the special case where there is only one philosopher.
 * 
 * How it works:
 * 1. It `locks philo->left_lock` only.
 * 2. Logs: “has taken a fork”
 * 3. Sleeps for `time_to_die`
 * 4. Unlocks the fork
 * 5. Then checks whether the simulation is still running
 * 6. If it is, it sets `sim->stop = true` and prints the death message
 * 
 * Why this is necessary:
 * * With only one philosopher, there is no second fork, 
 * so the philosopher can never eat.
 * * The simulation must end by force after `time_to_die`.
 */
void	single_philo_case(t_philo *philo)
{
	pthread_mutex_lock(philo->left_lock);
	log_action(philo, "has taken a fork");
	custom_usleep(philo->sim->time_to_die, philo->sim);
	pthread_mutex_unlock(philo->left_lock);
	if (!is_stopped(philo->sim))
	{
		pthread_mutex_lock(&philo->sim->print_lock);
		pthread_mutex_lock(&philo->sim->stop_lock);
		if (!philo->sim->stop)
		{
			philo->sim->stop = true;
			printf("%lld %d died\n",
				now_ms() - philo->sim->start_time_ms, philo->id);
		}
		pthread_mutex_unlock(&philo->sim->stop_lock);
		pthread_mutex_unlock(&philo->sim->print_lock);
	}
}

/**
 * Purpose: Defines the normal philosopher loop.
 * 
 * How it works:
 * * `while (!is_stopped(philo->sim))`
 * * Each iteration does:
 * 1. `lock_forks(philo)`
 * * picks up left and right fork
 * * logs fork acquisition
 * 2. if stop is triggered, break
 * 3. `philo_eat(philo)`
 * 4. `unlock_forks(philo)`
 * 5. if stop is triggered, break
 * 6. `log_action(philo, "is sleeping")`
 * 7. sleep for `time_to_sleep`
 * 8. if stop is triggered, break
 * 9. `log_action(philo, "is thinking")`
 * 
 * This is the full cycle:
 * 1. take forks
 * 2. eat
 * 3. release forks
 * 4. sleep
 * 5. think
 * 6. repeat
 */
void	philo_cycle(t_philo *philo)
{
	while (!is_stopped(philo->sim))
	{
		lock_forks(philo);
		if (is_stopped(philo->sim))
			break ;
		philo_eat(philo);
		unlock_forks(philo);
		if (is_stopped(philo->sim))
			break ;
		log_action(philo, "is sleeping");
		custom_usleep(philo->sim->time_to_sleep, philo->sim);
		if (is_stopped(philo->sim))
			break ;
		log_action(philo, "is thinking");
	}
}

/**
 * Purpose: This is the thread entry point for each philosopher.
 * 
 * How it works:
 * 1. It casts the void pointer back to `t_philo *`
 * 2. If philosopher id is even, it waits a tiny bit:
 * * `custom_usleep(1, philo->sim)`
 * * this helps avoid deadlock timing issues
 * 3. If `philo_count == 1`, it calls `single_philo_case`
 * 4. Otherwise it runs `philo_cycle`
 * 
 * So each philosopher thread starts here, and then follows the repeating cycle.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		custom_usleep(1, philo->sim);
	if (philo->sim->philo_count == 1)
	{
		single_philo_case(philo);
		return (NULL);
	}
	philo_cycle(philo);
	return (NULL);
}

/**
 * How the whole simulation fits together
 * 
 * The flow is:
 * 1. start_simulation() in simulation.c
 * * creates a monitor thread
 * * creates one thread per philosopher
 * 2. Each philosopher runs philo_routine()
 * * it takes forks, eats, sleeps, thinks
 * 3. The monitor thread in sim_monitor.c
 * * checks if someone died
 * * checks if all philosophers have eaten enough
 * * sets sim->stop = true when needed
 * 4. Helper functions in sim_utils.c
 * is_stopped()
 * set_stopped()
 * custom_usleep()
 * lock_forks()
 * unlock_forks()
 * 
 * These helpers make the logic safe under concurrency.
 */