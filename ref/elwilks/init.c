/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 21:10:48 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 15:31:44 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*:::::::::::::::::::::::    FORWARD DECLERATION    :::::::::::::::::::::::::*/

int	init_forks(t_config *config);
int	init_philos(t_config *config);

int	init_mutexes(t_config *config)
{
	if (pthread_mutex_init(&config->print_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&config->dead_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&config->meal_mutex, NULL))
		return (0);
	return (1);
}

int	init_fork_mutexes(t_config *config)
{
	int		i;

	config->forks = malloc(sizeof(pthread_mutex_t)
			* config->number_of_philosophers);
	if (config->forks == NULL)
		return (0);
	i = 0;
	while (i < config->number_of_philosophers)
	{
		if (pthread_mutex_init(&config->forks[i], NULL))
			return (0);
		i++;
	}
	return (1);
}

int	init_philos(t_config *config)
{
	int		i;

	config->philos = malloc(sizeof(t_philo) * config->number_of_philosophers);
	if (config->philos == NULL)
		return (0);
	i = 0;
	while (i < config->number_of_philosophers)
	{
		config->philos[i].id = i + 1;
		config->philos[i].last_meal_time = config->start_time;
		config->philos[i].meals_eaten = 0;
		config->philos[i].left_fork = &config->forks[i];
		config->philos[i].right_fork = &config->forks[(i + 1)
			% config->number_of_philosophers];
		config->philos[i].config = config;
		i++;
	}
	return (1);
}
