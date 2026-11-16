/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 17:36:16 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/16 17:40:48 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(t_sim *sim)
{
	int	index;

	if (sim->print_lock_init)
		pthread_mutex_destroy(&sim->print_lock);
	if (sim->state_lock_init)
		pthread_mutex_destroy(&sim->state_lock);
	if (sim->stop_lock_init)
		pthread_mutex_destroy(&sim->stop_lock);
	index = 0;
	while (index < sim->forks_init_count)
	{
		pthread_mutex_destroy(&sim->forks[index]);
		index++;
	}
	free(sim->philos);
	sim->philos = NULL;
	free(sim->forks);
	sim->forks = NULL;
}

int	alloc_sim(t_sim *sim)
{
	sim->philos = malloc(sizeof(t_philo) * sim->philo_count);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->philo_count);
	if (!sim->philos || !sim->forks)
	{
		free(sim->philos);
		sim->philos = NULL;
		free(sim->forks);
		sim->forks = NULL;
		return (0);
	}
	return (1);
}
