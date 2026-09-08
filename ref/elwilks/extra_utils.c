/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 15:38:08 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 15:38:11 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_config(t_config *config)
{
	printf("\nCONFIG SETUP\n");
	printf("Number of philosphers: %d\n", config->number_of_philosophers);
	printf("Time to Die: %lu\n", config->time_to_die);
	printf("Time to Eat: %lu\n", config->time_to_eat);
	printf("Time to Sleep: %lu\n", config->time_to_sleep);
	printf("Time to Sleep: %d\n", config->amount_to_eat);
	printf("Start Time: %lld\n", config->start_time);
	printf("Deaths Occured: %d\n\n", config->death_occurred);
}

void	print_philosphers(t_config *config)
{
	int		i;

	i = 0;
	while (i < config->number_of_philosophers)
	{
		printf("\nPhilospher ID: %d\n", config->philos[i].id);
		printf("Philospher %d Last Meal Time: %lu\n",
			config->philos[i].id, config->philos[i].last_meal_time);
		printf("Philospher %d Meals Eaten %d\n",
			config->philos[i].id, config->philos[i].meals_eaten);
		if (config->philos[i].left_fork != NULL)
			printf("Left Fork Ready\n");
		if (config->philos[i].right_fork != NULL)
			printf("Right Fork Ready\n");
		i++;
	}
	printf("\n");
}

void	print_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->config->print_mutex);
	printf("%lld %d died\n", get_time_ms()
		- philo->config->start_time, philo->id);
	pthread_mutex_unlock(&philo->config->print_mutex);
}

void	check_all_ate(t_philo *philo)
{
	int	i;
	int	all_done;

	all_done = 1;
	i = 0;
	while (i < philo->config->number_of_philosophers)
	{
		if (philo->config->philos[i].meals_eaten
			< philo->config->amount_to_eat)
			all_done = 0;
		i++;
	}
	if (all_done)
	{
		pthread_mutex_lock(&philo->config->dead_mutex);
		philo->config->death_occurred = 1;
		pthread_mutex_unlock(&philo->config->dead_mutex);
	}
}
