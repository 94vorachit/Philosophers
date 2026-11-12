/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:40:00 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 15:45:39 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_stopped(t_sim *sim)
{
	bool	stopped;

	pthread_mutex_lock(&sim->stop_lock);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stopped);
}

static void	set_stopped(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = true;
	pthread_mutex_unlock(&sim->stop_lock);
}

static void	log_action(t_philo *philo, const char *msg)
{
	long long	elapsed;

	pthread_mutex_lock(&philo->sim->print_lock);
	if (!is_stopped(philo->sim))
	{
		elapsed = now_ms() - philo->sim->start_time_ms;
		printf("%lld %d %s\n", elapsed, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->sim->print_lock);
}

static void	custom_usleep(long long ms, t_sim *sim)
{
	long long	limit;

	limit = now_ms() + ms;
	while (now_ms() < limit && !is_stopped(sim))
		usleep(500);
}

static void	lock_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_lock);
		log_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_lock);
		log_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_lock);
		log_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_lock);
		log_action(philo, "has taken a fork");
	}
}

static void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_lock);
	pthread_mutex_unlock(philo->right_lock);
}

static void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->state_lock);
	philo->last_meal_ms = now_ms();
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->sim->state_lock);
	log_action(philo, "is eating");
	custom_usleep(philo->sim->time_to_eat, philo->sim);
}

static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->sim->philo_count == 1)
	{
		pthread_mutex_lock(philo->left_lock);
		log_action(philo, "has taken a fork");
		custom_usleep(philo->sim->time_to_die, philo->sim);
		pthread_mutex_unlock(philo->left_lock);
		if (!is_stopped(philo->sim))
		{
			set_stopped(philo->sim);
			printf("%lld %d died\n", now_ms() - philo->sim->start_time_ms, philo->id);
		}
		return (NULL);
	}
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
	return (NULL);
}

static void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		index;
	bool	all_ate;
	long long	now;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		all_ate = true;
		index = 0;
		while (index < sim->philo_count)
		{
			pthread_mutex_lock(&sim->state_lock);
			now = now_ms();
			if (now - sim->philos[index].last_meal_ms > sim->time_to_die)
			{
				pthread_mutex_unlock(&sim->state_lock);
				set_stopped(sim);
				pthread_mutex_lock(&sim->print_lock);
				printf("%lld %d died\n", now - sim->start_time_ms, sim->philos[index].id);
				pthread_mutex_unlock(&sim->print_lock);
				return (NULL);
			}
			if (sim->must_eat >= 0 && sim->philos[index].meals_eaten < sim->must_eat)
				all_ate = false;
			pthread_mutex_unlock(&sim->state_lock);
			index++;
		}
		if (sim->must_eat >= 0 && all_ate)
		{
			set_stopped(sim);
			return (NULL);
		}
		usleep(500);
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
	pthread_join(sim->monitor, NULL);
	return (1);
}

void	cleanup_sim(t_sim *sim)
{
	int	index;

	if (sim->threads)
	{
		free(sim->threads);
		sim->threads = NULL;
	}
	index = 0;
	while (index < sim->philo_count)
	{
		pthread_mutex_destroy(&sim->forks[index]);
		index++;
	}
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->state_lock);
	pthread_mutex_destroy(&sim->stop_lock);
	if (sim->philos)
	{
		free(sim->philos);
		sim->philos = NULL;
	}
	if (sim->forks)
	{
		free(sim->forks);
		sim->forks = NULL;
	}
}
