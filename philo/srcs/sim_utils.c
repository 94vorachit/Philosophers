/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 18:41:15 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/16 18:42:07 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_stopped(t_sim *sim)
{
	bool	stopped;

	pthread_mutex_lock(&sim->stop_lock);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stopped);
}

void	set_stopped(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = true;
	pthread_mutex_unlock(&sim->stop_lock);
}

void	custom_usleep(long long ms, t_sim *sim)
{
	long long	limit;

	limit = now_ms() + ms;
	while (now_ms() < limit && !is_stopped(sim))
		usleep(500);
}

void	lock_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_lock);
	log_action(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_lock);
	log_action(philo, "has taken a fork");
}

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_lock);
	pthread_mutex_unlock(philo->right_lock);
}
