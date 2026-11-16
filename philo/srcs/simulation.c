/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 16:00:31 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/16 19:04:07 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Group 4: thread creation and startup
*/
static int	create_threads(t_sim *sim)
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
