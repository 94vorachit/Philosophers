/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 16:00:31 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/17 16:23:32 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Purpose: Creates one pthread for every philosopher.
 * 
 * How it works
 * 1. starts with `index = 0`
 * 2. while `index < sim->philo_count`:
 * * calls `pthread_create(...)`
 * * each thread runs `philo_routine`
 * * each thread gets a pointer to one philosopher: `&sim->philos[index]`
 * 3. if thread creation fails:
 * * set the stop flag
 * * join the monitor thread
 * * join any philosopher threads that were already created
 * * free the thread array
 * * return `0`
 * 
 * Why this matters
 * The simulation cannot start correctly unless each philosopher has 
 * its own execution thread.
 */
int	create_threads(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->philo_count)
	{
		if (pthread_create(&sim->threads[index], NULL, philo_routine,
				&sim->philos[index]) != 0)
		{
			set_stopped(sim);
			pthread_join(sim->monitor, NULL);
			while (index > 0)
			{
				index--;
				pthread_join(sim->threads[index], NULL);
			}
			free(sim->threads);
			sim->threads = NULL;
			return (0);
		}
		index++;
	}
	return (1);
}

/**
 * Purpose: Starts the simulation and waits until it ends.
 * 
 * How it works:
 * 1. allocate memory for `sim->threads`:
 * * `malloc(sizeof(pthread_t) * sim->philo_count);`
 * 2. if allocation fails, return `0`
 * 3. create the monitor thread:
 * * `pthread_create(&sim->monitor, NULL, monitor_routine, sim)`
 * 4. if monitor creation fails, free thread array and return `0`
 * 5. call `create_threads(sim)`
 * 6. wait for all philosopher threads to finish:
 * * `pthread_join(sim->threads[index], NULL);`
 * 7. wait for the monitor thread to finish:
 * * `pthread_join(sim->monitor, NULL);`
 * 8. return `1`
 * 
 * Why this matters
 * This is the main orchestration point: it tells the system to begin 
 * all threads and then blocks until the simulation is finished.
 */
int	start_simulation(t_sim *sim)
{
	int	index;

	sim->threads = malloc(sizeof(pthread_t) * sim->philo_count);
	if (!sim->threads)
		return (0);
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
	{
		free(sim->threads);
		sim->threads = NULL;
		return (0);
	}
	if (!create_threads(sim))
		return (0);
	index = 0;
	while (index < sim->philo_count)
	{
		pthread_join(sim->threads[index], NULL);
		index++;
	}
	pthread_join(sim->monitor, NULL);
	return (1);
}

/**
 * Overall workflow
 * 
 * The system starts like this:
 * 1. memory for thread array is allocated
 * 2. monitor thread is created
 * 3. philosopher threads are created
 * 4. philosophers run their routines
 * 5. monitor checks if a philosopher died or if everyone ate enough
 * 6. when the simulation ends, the main thread joins all worker threads
 * 7. the program exits cleanly
 */