/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:00:00 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 14:51:37 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo(t_sim *sim, int idx)
{
	sim->philos[idx].id = idx + 1;
	sim->philos[idx].left_fork = idx;
	sim->philos[idx].right_fork = (idx + 1) % sim->philo_count;
	sim->philos[idx].last_meal_ms = sim->start_time_ms;
	sim->philos[idx].meals_eaten = 0;
	sim->philos[idx].left_lock = &sim->forks[idx];
	sim->philos[idx].right_lock = &sim->forks[sim->philos[idx].right_fork];
	sim->philos[idx].sim = sim;
}

static void	destroy_mutexes(t_sim *sim)
{
	int	idx;

	idx = 0;
	while (idx < sim->philo_count)
	{
		pthread_mutex_destroy(&sim->forks[idx]);
		idx++;
	}
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->state_lock);
	pthread_mutex_destroy(&sim->stop_lock);
}

int	init_sim(t_sim *sim)
{
	int	idx;

	sim->start_time_ms = now_ms();
	sim->stop = false;
	sim->philos = malloc(sizeof(t_philo) * sim->philo_count);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->philo_count);
	if (!sim->philos || !sim->forks)
		return (0);
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->state_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
		return (0);
	idx = 0;
	while (idx < sim->philo_count)
	{
		if (pthread_mutex_init(&sim->forks[idx], NULL) != 0)
		{
			destroy_mutexes(sim);
			return (0);
		}
		idx++;
	}
	idx = 0;
	while (idx < sim->philo_count)
	{
		init_philo(sim, idx);
		idx++;
	}
	return (1);
}
