/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 18:41:15 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 21:25:11 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Purpose: Check whether the whole simulation should stop.
 * 
 * How it works:
 * 1. It locks `sim->stop_lock`
 * 2. Reads `sim->stop`
 * 3. Unlocks the mutex
 * 4. Returns the value
 * 
 * Why this matters:
 * * Multiple threads may try to stop the simulation at the same time.
 * * The mutex ensures the value is read safely without race conditions.
 * 
 * This is a classic thread-safe read.
 */
bool	is_stopped(t_sim *sim)
{
	bool	stopped;

	pthread_mutex_lock(&sim->stop_lock);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stopped);
}

/**
 * Purpose: Mark the simulation as ended.
 * 
 * How it works:
 * 1. Locks `sim->stop_lock`
 * 2. Sets `sim->stop = true`
 * 3. Unlocks the mutex
 * 
 * Why this matters:
 * * When a philosopher dies or all philosophers finish eating, 
 * the simulation must stop for all threads.
 * * This function is the centralized way to end the run.
 */
void	set_stopped(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = true;
	pthread_mutex_unlock(&sim->stop_lock);
}

/**
 * Purpose: Sleep for a given time, but stop early if 
 * the simulation is already ended.
 * 
 * How it works:
 * 1. Computes `limit = now_ms() + ms`
 * 2. Repeats:
 * * check current time
 * * keep sleeping while current time is before the limit
 * * stop immediately if `is_stopped(sim)` becomes true
 * 3. Uses `usleep(100)` in each loop to avoid blocking too long
 * 
 * Why this matters:
 * * Normal `usleep()` blocks for the entire duration, 
 * * It respects the stop flag without wasting time.
 * 
 * This is important in a simulation where one philosopher may die 
 * and all others must stop promptly.
 */
void	custom_usleep(long long ms, t_sim *sim)
{
	long long	limit;

	limit = now_ms() + ms;
	while (now_ms() < limit && !is_stopped(sim))
		usleep(100);
}

/**
 * Purpose: Release both fork mutexes after eating.
 * 
 * How it works:
 * 1. Unlock left fork
 * 2. Unlock right fork
 * 
 * Why this matters:
 * * Once the philosopher finishes eating, 
 * the forks must be available to neighbors again.
 * * Without unlocking, the simulation would deadlock.
 */
void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_lock);
	pthread_mutex_unlock(philo->right_lock);
}

/**
 * The full philosopher cycle is:
 * 
 * 1. `lock_forks(philo)`
 * 2. `philo_eat(philo)`
 * 3. `unlock_forks(philo)`
 * 4. sleep
 * 5. think
 * 6. repeat until simulation stops
 * 
 * The helpers in this file are the “control layer” for that logic:
 * `is_stopped` checks if all work should end
 * `set_stopped` ends the simulation
 * `custom_usleep` waits safely
 * `lock_forks` and `unlock_forks` manage shared resources
 * 
 * These are essential for preventing deadlock, race conditions, 
 * and uncontrolled thread execution.
 */