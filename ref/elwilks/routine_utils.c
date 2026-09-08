/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 15:33:57 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 15:34:47 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->config->time_to_sleep);
}

void	think(t_philo *philo)
{
	print_status(philo, "is thinking");
}

int	eaten_enough(t_philo *philo)
{
	if (philo->config->amount_to_eat == -1)
		return (0);
	if (philo->meals_eaten >= philo->config->amount_to_eat)
		return (1);
	return (0);
}

void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->config->time_to_die);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&philo->config->dead_mutex);
	philo->config->death_occurred = 1;
	pthread_mutex_unlock(&philo->config->dead_mutex);
	print_death(philo);
	return (NULL);
}
