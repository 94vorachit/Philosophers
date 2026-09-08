/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 21:11:39 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 14:30:57 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philosopher_death(t_config *config, int i)
{
	long long	time_since;

	pthread_mutex_lock(&config->meal_mutex);
	time_since = get_time_ms() - config->philos[i].last_meal_time;
	pthread_mutex_unlock(&config->meal_mutex);
	if (time_since > config->time_to_die)
	{
		pthread_mutex_lock(&config->dead_mutex);
		if (!config->death_occurred)
		{
			config->death_occurred = 1;
			pthread_mutex_unlock(&config->dead_mutex);
			print_death(&config->philos[i]);
		}
		else
			pthread_mutex_unlock(&config->dead_mutex);
		return (1);
	}
	return (0);
}

static int	check_all_meals_complete(t_config *config)
{
	int	i;
	int	all_ate;

	if (config->amount_to_eat == -1)
		return (0);
	all_ate = 1;
	i = 0;
	while (i < config->number_of_philosophers)
	{
		pthread_mutex_lock(&config->meal_mutex);
		if (config->philos[i].meals_eaten < config->amount_to_eat)
			all_ate = 0;
		pthread_mutex_unlock(&config->meal_mutex);
		if (!all_ate)
			break ;
		i++;
	}
	if (all_ate)
	{
		pthread_mutex_lock(&config->dead_mutex);
		config->death_occurred = 1;
		pthread_mutex_unlock(&config->dead_mutex);
		return (1);
	}
	return (0);
}

void	*monitor(void *config_struct)
{
	int			i;
	t_config	*config;

	config = (t_config *)config_struct;
	while (1)
	{
		i = 0;
		while (i < config->number_of_philosophers)
		{
			if (check_philosopher_death(config, i))
				return (NULL);
			i++;
		}
		if (check_all_meals_complete(config))
			return (NULL);
		simple_usleep(1);
	}
	return (NULL);
}
