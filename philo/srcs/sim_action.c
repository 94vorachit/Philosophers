/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_action.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 19:02:07 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/16 19:02:59 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_action(t_philo *philo, const char *msg)
{
	long long	elapsed;
	bool		stopped;

	pthread_mutex_lock(&philo->sim->stop_lock);
	stopped = philo->sim->stop;
	pthread_mutex_unlock(&philo->sim->stop_lock);
	if (stopped)
		return ;
	pthread_mutex_lock(&philo->sim->print_lock);
	pthread_mutex_lock(&philo->sim->stop_lock);
	stopped = philo->sim->stop;
	pthread_mutex_unlock(&philo->sim->stop_lock);
	if (!stopped)
	{
		elapsed = now_ms() - philo->sim->start_time_ms;
		printf("%lld %d %s\n", elapsed, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->sim->print_lock);
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->state_lock);
	philo->last_meal_ms = now_ms();
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->sim->state_lock);
	log_action(philo, "is eating");
	custom_usleep(philo->sim->time_to_eat, philo->sim);
}

void	single_philo_case(t_philo *philo)
{
	pthread_mutex_lock(philo->left_lock);
	log_action(philo, "has taken a fork");
	custom_usleep(philo->sim->time_to_die, philo->sim);
	pthread_mutex_unlock(philo->left_lock);
	if (!is_stopped(philo->sim))
	{
		pthread_mutex_lock(&philo->sim->print_lock);
		pthread_mutex_lock(&philo->sim->stop_lock);
		if (!philo->sim->stop)
		{
			philo->sim->stop = true;
			printf("%lld %d died\n",
				now_ms() - philo->sim->start_time_ms, philo->id);
		}
		pthread_mutex_unlock(&philo->sim->stop_lock);
		pthread_mutex_unlock(&philo->sim->print_lock);
	}
}

void	philo_cycle(t_philo *philo)
{
	while (!is_stopped(philo->sim))
	{
		lock_forks(philo);
		if (is_stopped(philo->sim))
			break ;
		philo_eat(philo);
		unlock_forks(philo);
		if (is_stopped(philo->sim))
			break ;
		log_action(philo, "is sleeping");
		custom_usleep(philo->sim->time_to_sleep, philo->sim);
		if (is_stopped(philo->sim))
			break ;
		log_action(philo, "is thinking");
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		custom_usleep(1, philo->sim);
	if (philo->sim->philo_count == 1)
	{
		single_philo_case(philo);
		return (NULL);
	}
	philo_cycle(philo);
	return (NULL);
}
