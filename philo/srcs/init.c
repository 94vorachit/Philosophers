/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:00:00 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 19:00:02 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->philo_count)
	{
		pthread_mutex_destroy(&sim->forks[index]);
		index++;
	}
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->state_lock);
	pthread_mutex_destroy(&sim->stop_lock);
}

int	init_shared_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->state_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
		return (0);
	return (1);
}

int	init_forks(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->philo_count)
	{
		if (pthread_mutex_init(&sim->forks[index], NULL) != 0)
		{
			destroy_mutexes(sim);
			return (0);
		}
		index++;
	}
	return (1);
}

void	init_philos(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->philo_count)
	{
		sim->philos[index].id = index + 1;
		sim->philos[index].left_fork = index;
		sim->philos[index].right_fork = (index + 1) % sim->philo_count;
		sim->philos[index].last_meal_ms = sim->start_time_ms;
		sim->philos[index].meals_eaten = 0;
		sim->philos[index].left_lock = &sim->forks[index];
		sim->philos[index].right_lock
			= &sim->forks[sim->philos[index].right_fork];
		sim->philos[index].sim = sim;
		index++;
	}
}

int	init_sim(t_sim *sim)
{
	sim->start_time_ms = now_ms();
	sim->stop = false;
	sim->philos = malloc(sizeof(t_philo) * sim->philo_count);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->philo_count);
	if (!sim->philos || !sim->forks)
	{
		free(sim->philos);
		free(sim->forks);
		sim->philos = NULL;
		sim->forks = NULL;
		return (0);
	}
	if (!init_shared_mutexes(sim))
	{
		free(sim->philos);
		free(sim->forks);
		sim->philos = NULL;
		sim->forks = NULL;
		return (0);
	}
	if (!init_forks(sim))
	{
		destroy_mutexes(sim);
		free(sim->philos);
		free(sim->forks);
		sim->philos = NULL;
		sim->forks = NULL;
		return (0);
	}
	init_philos(sim);
	return (1);
}
