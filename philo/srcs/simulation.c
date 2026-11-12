/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 16:00:31 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 16:55:06 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Add a single helper to read the stop flag safely, and use it in a thread 
 * that exits after a few iterations.
 * 
 * it adds safe stop-state checking
 * it prevents the infinite loop while still using the stop flag
 * it keeps the thread logic minimal and testable
 * it is the exact pattern you will reuse in the real philosopher loop later
 */
static bool	is_stopped(t_sim *sim)
{
	bool	stopped;

	pthread_mutex_lock(&sim->stop_lock);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stopped);
}

/**
 * Add one small helper that sets the stop flag after a short finite loop, 
 * then the philosopher thread exits normally.
 * 
 * it avoids the infinite wait
 * it proves the thread can terminate
 * it keeps the logic completely separate from eating/death rules
 * it is the exact “exit condition” pattern the real simulation will need later
 */
static void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = true;
	pthread_mutex_unlock(&sim->stop_lock);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	i = 0;
	while (i < 3 && !is_stopped(philo->sim))
	{
		usleep(1000);
		i++;
	}
	set_stop(philo->sim);
	return (NULL);
}

int	start_simulation(t_sim *sim)
{
	int	index;

	sim->threads = malloc(sizeof(pthread_t) * sim->philo_count);
	if (!sim->threads)
		return (0);
	index = 0;
	while (index < sim->philo_count)
	{
		if (pthread_create(&sim->threads[index], NULL, philo_routine,
				&sim->philos[index]) != 0)
			return (0);
		index++;
	}
	index = 0;
	while (index < sim->philo_count)
	{
		pthread_join(sim->threads[index], NULL);
		index++;
	}
	free(sim->threads);
	sim->threads = NULL;
	return (1);
}
