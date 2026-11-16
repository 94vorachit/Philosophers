/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:00:00 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/16 14:44:30 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo(t_sim *sim, int index)
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
}

int	init_sim(t_sim *sim)
{
	int	index;

	sim->start_time_ms = now_ms();
	sim->stop = false;
	sim->print_lock_init = false;
	sim->state_lock_init = false;
	sim->stop_lock_init = false;
	sim->forks_init_count = 0;
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
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
	{
		free(sim->philos);
		sim->philos = NULL;
		free(sim->forks);
		sim->forks = NULL;
		return (0);
	}
	sim->print_lock_init = true;
	if (pthread_mutex_init(&sim->state_lock, NULL) != 0)
	{
		destroy_mutexes(sim);
		free(sim->philos);
		sim->philos = NULL;
		free(sim->forks);
		sim->forks = NULL;
		return (0);
	}
	sim->state_lock_init = true;
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
	{
		destroy_mutexes(sim);
		free(sim->philos);
		sim->philos = NULL;
		free(sim->forks);
		sim->forks = NULL;
		return (0);
	}
	sim->stop_lock_init = true;
	index = 0;
	while (index < sim->philo_count)
	{
		if (pthread_mutex_init(&sim->forks[index], NULL) != 0)
		{
			sim->forks_init_count = index;
			destroy_mutexes(sim);
			free(sim->philos);
			sim->philos = NULL;
			free(sim->forks);
			sim->forks = NULL;
			return (0);
		}
		sim->forks_init_count = index + 1;
		index++;
	}
	index = 0;
	while (index < sim->philo_count)
	{
		init_philo(sim, index);
		index++;
	}
	return (1);
}
