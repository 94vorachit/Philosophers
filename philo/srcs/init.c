/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:00:00 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/16 18:04:16 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Purpose: Initializes one philosopher
 * 
 * What it does:
 * `id = index + 1`
 * * Each philosopher gets a unique number starting from 1.
 * `left_fork = index`
 * * Philosopher 0 uses fork 0 as left fork.
 * `right_fork = (index + 1) % sim->philo_count`
 * * The last philosopher wraps around to fork 0, creating the circular table.
 * `last_meal_ms = sim->start_time_ms`
 * * The philosopher starts as if they just ate at the start.
 * `meals_eaten = 0`
 * * No meals yet.
 * `left_lock = &sim->forks[index]`
 * * Each philosopher points to its left fork mutex.
 * `right_lock = &sim->forks[sim->philos[index].right_fork]`
 * * Each philosopher points to its right fork mutex.
 * `sim = sim`
 * * The philosopher holds a pointer back to the shared simulation data.
 */
void	init_philo(t_sim *sim, int index)
{
	sim->philos[index].id = index + 1;
	sim->philos[index].left_fork = index;
	sim->philos[index].right_fork = (index + 1) % sim->philo_count;
	sim->philos[index].last_meal_ms = sim->start_time_ms;
	sim->philos[index].meals_eaten = 0;
	sim->philos[index].left_lock = &sim->forks[index];
	sim->philos[index].right_lock = &sim->forks[sim->philos[index].right_fork];
	sim->philos[index].sim = sim;
}

/**
 * Purpose: This initializes the shared mutexes used by the whole program.
 * 
 * Why these exist:
 * * `print_lock`: protects printf output so messages do not mix together
 * * `state_lock`: protects shared philosopher state like 
 * last_meal_ms and meals_eaten
 * * `stop_lock`: protects the stop flag so multiple threads don’t race 
 * on shutdown
 * 
 * This is important because later cleanup code destroys only the mutexes 
 * that were successfully initialized.
 */
int	init_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (0);
	sim->print_lock_init = true;
	if (pthread_mutex_init(&sim->state_lock, NULL) != 0)
		return (0);
	sim->state_lock_init = true;
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
		return (0);
	sim->stop_lock_init = true;
	return (1);
}

/**
 * Purpose: This initializes every fork mutex.
 * 
 * How it works:
 * 1. loop from `0` to `sim->philo_count - 1`
 * 2. call `pthread_mutex_init(&sim->forks[index], NULL)`
 * 3. if one fails, store the failed index in `forks_init_count`
 * 4. return `0`
 * 
 * This ensures the fork mutexes are ready before any philosopher starts 
 * locking them.
 */
int	init_forks(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->philo_count)
	{
		if (pthread_mutex_init(&sim->forks[index], NULL) != 0)
		{
			sim->forks_init_count = index;
			return (0);
		}
		sim->forks_init_count = index + 1;
		index++;
	}
	return (1);
}

/**
 * Purpose: This resets the simulation state before initialization.
 * 
 * It sets:
 * 1. `start_time_ms = now_ms()`
 * Current time when the simulation starts
 * 2. `stop = false`
 * No one died yet, simulation still running
 * 3. all `*_init` flags to `false`
 * 4. `forks_init_count = 0`
 * 
 * This is a clean start state.
 */
void	set_init_sim(t_sim *sim)
{
	sim->start_time_ms = now_ms();
	sim->stop = false;
	sim->print_lock_init = false;
	sim->state_lock_init = false;
	sim->stop_lock_init = false;
	sim->forks_init_count = 0;
}

/**
 * Purpose: This is the main setup function that orchestrates everything.
 * 
 * Flow:
 * 1. `set_init_sim(sim)`
 * 2. `alloc_sim(sim)`
 * allocate memory for philosopher array and fork array
 * 3. `init_mutexes(sim)`
 * create shared locks
 * 4. `init_forks(sim)`
 * create all fork locks
 * 5. loop through each philosopher and call `init_philo(sim, index)`
 * 
 * Finally:
 * * returns `1` on success
 * * returns `0` on failure
 * 
 * This is the “master initializer” for the simulation.
 */
int	init_sim(t_sim *sim)
{
	int	index;

	set_init_sim(sim);
	if (!alloc_sim(sim))
		return (0);
	if (!init_mutexes(sim))
	{
		destroy_mutexes(sim);
		return (0);
	}
	if (!init_forks(sim))
	{
		destroy_mutexes(sim);
		return (0);
	}
	index = 0;
	while (index < sim->philo_count)
	{
		init_philo(sim, index);
		index++;
	}
	return (1);
}
