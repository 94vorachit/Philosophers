/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 16:00:31 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 16:10:57 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_stopped(t_sim *sim)
{
	bool	stopped;

	pthread_mutex_lock(&sim->stop_lock);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stopped);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		count;

	philo = (t_philo *)arg;
	count = 0;
	while (count < 3)
	{
		usleep(1000);
		count++;
	}
	pthread_mutex_lock(&philo->sim->stop_lock);
	philo->sim->stop = true;
	pthread_mutex_unlock(&philo->sim->stop_lock);
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
