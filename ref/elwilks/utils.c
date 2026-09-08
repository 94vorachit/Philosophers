/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 21:13:13 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 16:08:34 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	long				current_time;
	struct timeval		time;

	gettimeofday(&time, NULL);
	current_time = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (current_time);
}

void	print_status(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->config->print_mutex);
	pthread_mutex_lock(&philo->config->dead_mutex);
	if (philo->config->death_occurred != 1)
	{
		printf("%llu %d %s\n", get_time_ms()
			- philo->config->start_time, philo->id, str);
	}
	pthread_mutex_unlock(&philo->config->print_mutex);
	pthread_mutex_unlock(&philo->config->dead_mutex);
}

void	cleanup(t_config *config)
{
	int	i;

	i = 0;
	while (i < config->number_of_philosophers)
	{
		pthread_mutex_destroy(&config->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&config->print_mutex);
	pthread_mutex_destroy(&config->dead_mutex);
	pthread_mutex_destroy(&config->meal_mutex);
	free(config->forks);
	free(config->philos);
	free(config->threads);
}

void	ft_usleep(int ms)
{
	long long	start;
	long long	current;

	start = get_time_ms();
	while (1)
	{
		current = get_time_ms();
		if (current - start >= ms)
			break ;
		usleep(100);
	}
}

void	simple_usleep(size_t ms)
{
	size_t	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
	{
		usleep(500);
	}
}
