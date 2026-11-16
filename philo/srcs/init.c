/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:00:00 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/16 17:37:41 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	set_init_sim(t_sim *sim)
{
	sim->start_time_ms = now_ms();
	sim->stop = false;
	sim->print_lock_init = false;
	sim->state_lock_init = false;
	sim->stop_lock_init = false;
	sim->forks_init_count = 0;
}

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
