/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 16:00:31 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 18:36:39 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Add a single helper to read the stop flag safely, and use it in a thread 
 * that exits after a few iterations.
 * 
 * it adds safe stop-state checking
 * it prevents the infinite loop while still using the stop flag
 * it keeps the thread logic minimal and testable
 * it is the exact pattern you will reuse in the real philosopher loop later
 */
bool	is_stopped(t_sim *sim)
{
	bool	stopped;

	pthread_mutex_lock(&sim->stop_lock);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stopped);
}

/**
 * Add one small helper that sets the stop flag after a short finite loop, 
 * then the philosopher thread exits normally.
 * 
 * it avoids the infinite wait
 * it proves the thread can terminate
 * it keeps the logic completely separate from eating/death rules
 * it is the exact “exit condition” pattern the real simulation will need later
 */
// void	set_stop(t_sim *sim)
// {
// 	pthread_mutex_lock(&sim->stop_lock);
// 	sim->stop = true;
// 	pthread_mutex_unlock(&sim->stop_lock);
// }

void	lock_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_lock);
	pthread_mutex_lock(philo->right_lock);
}

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_lock);
	pthread_mutex_unlock(philo->right_lock);
}

/**
 * Add the log function and print each action with a timestamp.
 */
void	log_action(t_philo *philo, const char *action)
{
	long long	elapsed;

	pthread_mutex_lock(&philo->sim->print_lock);
	elapsed = now_ms() - philo->sim->start_time_ms;
	printf("%lld %d %s\n", elapsed, philo->id, action);
	pthread_mutex_unlock(&philo->sim->print_lock);
}

/**
 * Add the last_meal_ms update right before the philosopher starts eating.
 */
void	eat_action(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->state_lock);
	philo->last_meal_ms = now_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->sim->state_lock);

	log_action(philo, "is eating");
	usleep(philo->sim->time_to_eat * 1000);
}

/**
 * Add the completion condition for must_eat in the monitor.
 */
int	all_philos_ate(t_sim *sim)
{
    int	i;

    i = 0;
    while (i < sim->philo_count)
    {
        pthread_mutex_lock(&sim->state_lock);
        if (sim->philos[i].meals_eaten < sim->must_eat)
        {
            pthread_mutex_unlock(&sim->state_lock);
            return (0);
        }
        pthread_mutex_unlock(&sim->state_lock);
        i++;
    }
    return (1);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		lock_forks(philo);
		eat_action(philo);
		unlock_forks(philo);

		log_action(philo, "is sleeping");
		usleep(philo->sim->time_to_sleep * 1000);

		log_action(philo, "is thinking");
	}
	return (NULL);
}

/**
 * Add the monitor thread before the full death logic, 
 * with a very small fatal check.
 * 
 * This is the first real monitor loop:
 * * it watches each philosopher
 * * it transitions the simulation to stop
 * * it is the foundation for the actual death detection logic
 */
void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		i = 0;
		while (i < sim->philo_count)
		{
			pthread_mutex_lock(&sim->state_lock);
			if (now_ms() - sim->philos[i].last_meal_ms > sim->time_to_die)
			{
				pthread_mutex_unlock(&sim->state_lock);
				pthread_mutex_lock(&sim->print_lock);
				printf("%lld %d died\n",
					now_ms() - sim->start_time_ms, sim->philos[i].id);
				pthread_mutex_unlock(&sim->print_lock);
				pthread_mutex_lock(&sim->stop_lock);
				sim->stop = true;
				pthread_mutex_unlock(&sim->stop_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&sim->state_lock);
			i++;
			if (sim->must_eat > 0 && all_philos_ate(sim))
			{
				pthread_mutex_lock(&sim->stop_lock);
				sim->stop = true;
				pthread_mutex_unlock(&sim->stop_lock);
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}

int	start_simulation(t_sim *sim)
{
	int	index;

	sim->threads = malloc(sizeof(pthread_t) * sim->philo_count);
	if (!sim->threads)
		return (0);
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
		return (0);
	index = 0;
	while (index < sim->philo_count)
	{
		if (pthread_create(&sim->threads[index], NULL, philo_routine,
				&sim->philos[index]) != 0)
			return (0);
		index++;
	}
	index = 0;
	while (index < sim->philo_count)
	{
		pthread_join(sim->threads[index], NULL);
		index++;
	}
	free(sim->threads);
	sim->threads = NULL;
	return (1);
}
