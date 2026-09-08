/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 15:33:03 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 15:34:45 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	died(t_philo *philo)
{
	int		dead;

	pthread_mutex_lock(&philo->config->dead_mutex);
	dead = philo->config->death_occurred;
	pthread_mutex_unlock(&philo->config->dead_mutex);
	return (dead);
}

void	pickup_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
}

void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	eat(t_philo *philo)
{
	pickup_forks(philo);
	pthread_mutex_lock(&philo->config->meal_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->config->meal_mutex);
	print_status(philo, "is eating");
	ft_usleep(philo->config->time_to_eat);
	pthread_mutex_lock(&philo->config->meal_mutex);
	philo->meals_eaten++;
	if (philo->config->amount_to_eat != -1
		&& philo->meals_eaten >= philo->config->amount_to_eat)
		check_all_ate(philo);
	pthread_mutex_unlock(&philo->config->meal_mutex);
	put_down_forks(philo);
}

void	*routine(void *philos)
{
	t_philo		*philo;

	philo = (t_philo *)philos;
	if (philo->config->number_of_philosophers == 1)
		return (one_philo_routine(philo));
	if (philo->id % 2 == 0)
		ft_usleep(philo->config->time_to_eat / 2);
	while (died(philo) != 1)
	{
		eat(philo);
		if (eaten_enough(philo))
			break ;
		philo_sleep(philo);
		think(philo);
	}
	return (NULL);
}
