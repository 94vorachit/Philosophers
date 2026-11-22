/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 16:35:24 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 19:12:04 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Purpose: Choose the two fork mutexes for a philosopher in 
 * ascending fork-index order.
 * 
 * How it works: 
 * * Compare left_fork/right_fork, then assign first and second pointers.
 * * The fixed order is used to prevent circular lock acquisition.
 */
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
 * Purpose: Lock both forks required for one eating turn.
 * 
 * How it works:
 * 1. Resolve both forks in stable index order.
 * 2. Apply odd-table startup delay when needed.
 * 3. Lock the first fork and stop early if shutdown begins.
 * 4. Lock the second fork and log both fork acquisitions.
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

/**
 * Overall workflow:
 * 1. Resolve each philosopher's two fork mutex pointers.
 * 2. Acquire the lower fork index first to prevent deadlock cycles.
 * 3. Log each successful fork acquisition.
 * 4. Eating and fork release are handled by sim_action.c and sim_utils.c.
 */
