/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_monitor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 19:03:25 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/16 19:03:58 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	all_ate(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->philo_count)
	{
		pthread_mutex_lock(&sim->state_lock);
		if (sim->philos[index].meals_eaten < sim->must_eat)
		{
			pthread_mutex_unlock(&sim->state_lock);
			return (false);
		}
		pthread_mutex_unlock(&sim->state_lock);
		index++;
	}
	return (true);
}

bool	check_death(t_sim *sim, int index, long long now)
{
	if (now - sim->philos[index].last_meal_ms <= sim->time_to_die)
		return (false);
	pthread_mutex_unlock(&sim->state_lock);
	pthread_mutex_lock(&sim->print_lock);
	pthread_mutex_lock(&sim->stop_lock);
	if (!sim->stop)
	{
		sim->stop = true;
		printf("%lld %d died\n",
			now - sim->start_time_ms, sim->philos[index].id);
	}
	pthread_mutex_unlock(&sim->stop_lock);
	pthread_mutex_unlock(&sim->print_lock);
	return (true);
}

void	monitor_cycle(t_sim *sim)
{
	int			index;
	long long	now;

	index = 0;
	while (index < sim->philo_count)
	{
		pthread_mutex_lock(&sim->state_lock);
		now = now_ms();
		if (sim->must_eat >= 0
			&& sim->philos[index].meals_eaten >= sim->must_eat)
		{
			pthread_mutex_unlock(&sim->state_lock);
			index++;
			continue ;
		}
		if (now - sim->philos[index].last_meal_ms > sim->time_to_die)
		{
			if (check_death(sim, index, now))
				return ;
			return ;
		}
		pthread_mutex_unlock(&sim->state_lock);
		index++;
	}
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		if (sim->must_eat >= 0 && all_ate(sim))
		{
			set_stopped(sim);
			return (NULL);
		}
		monitor_cycle(sim);
		usleep(500);
	}
	return (NULL);
}
