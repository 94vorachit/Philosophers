/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 16:35:24 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 18:01:02 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_forks_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_lock;
		*second = philo->right_lock;
	}
	else
	{
		*first = philo->right_lock;
		*second = philo->left_lock;
	}
}

/**
 * Purpose: Take the philosopher’s left and right forks.
 * 
 * How it works:
 * 1. `pthread_mutex_lock(philo->left_lock);`
 * 2. logs: “has taken a fork”
 * 3. `pthread_mutex_lock(philo->right_lock);`
 * 4. logs: “has taken a fork”
 * 
 * Why this matters:
 * * Each philosopher needs both forks to eat.
 * * Locking them prevents two philosophers from grabbing 
 * the same fork at the same time.
 * 
 * This is the key concurrency rule in the Dining Philosophers problem.
 */
void	lock_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (is_stopped(philo->sim))
		return ;
	lock_forks_order(philo, &first, &second);
	if (philo->sim->philo_count % 2 == 1 && philo->id % 2 == 0)
		custom_usleep(1, philo->sim);
	pthread_mutex_lock(first);
	if (is_stopped(philo->sim))
	{
		pthread_mutex_unlock(first);
		return ;
	}
	log_action(philo, "has taken a fork");
	pthread_mutex_lock(second);
	if (is_stopped(philo->sim))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return ;
	}
	log_action(philo, "has taken a fork");
}
