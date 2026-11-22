/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 15:00:41 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 15:23:20 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_stopped(t_philo *philo)
{
	bool	stopped;

	pthread_mutex_lock(philo->mutexes.meal_lock);
	stopped = *philo->stop;
	pthread_mutex_unlock(philo->mutexes.meal_lock);
	return (stopped);
}

static void	set_stopped(t_philo *philos)
{
	pthread_mutex_lock(philos->mutexes.meal_lock);
	*philos->stop = true;
	pthread_mutex_unlock(philos->mutexes.meal_lock);
}

bool	is_all_eat(t_philo *philos)
{
	int	finished;
	int	i;

	if (philos[0].must_eat == -1)
		return (false);
	finished = 0;
	pthread_mutex_lock(philos->mutexes.meal_lock);
	i = -1;
	while (++i < philos[0].philo_count)
	{
		if (philos[i].meals_eaten >= philos[i].must_eat)
			++finished;
	}
	pthread_mutex_unlock(philos->mutexes.meal_lock);
	return (finished == philos[0].philo_count);
}

void	*obsorver(void *ptr)
{
	t_philo	*philos;
	int		i;
	size_t	now;

	philos = (t_philo *)ptr;
	while (!is_stopped(philos))
	{
		i = -1;
		while (++i < philos[0].philo_count)
		{
			pthread_mutex_lock(philos->mutexes.meal_lock);
			now = get_current_time();
			if (now - philos[i].times.last_meal > philos[i].times.die)
			{
				pthread_mutex_unlock(philos->mutexes.meal_lock);
				print_action(&philos[i], RED" died"RESET);
				set_stopped(philos);
				return (NULL);
			}
			pthread_mutex_unlock(philos->mutexes.meal_lock);
		}
		if (is_all_eat(philos))
		{
			set_stopped(philos);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	if (philo->philo_count == 1)
	{
		pthread_mutex_lock(philo->mutexes.left_fork);
		print_action(philo, " has taken a fork");
		while (!is_stopped(philo))
			usleep(500);
		pthread_mutex_unlock(philo->mutexes.left_fork);
		return;
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->mutexes.right_fork);
		if (is_stopped(philo))
		{
			pthread_mutex_unlock(philo->mutexes.right_fork);
			return;
		}
		print_action(philo, " has taken a fork");
		pthread_mutex_lock(philo->mutexes.left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->mutexes.left_fork);
		if (is_stopped(philo))
		{
			pthread_mutex_unlock(philo->mutexes.left_fork);
			return;
		}
		print_action(philo, " has taken a fork");
		pthread_mutex_lock(philo->mutexes.right_fork);
	}
	if (is_stopped(philo))
	{
		pthread_mutex_unlock(philo->mutexes.left_fork);
		pthread_mutex_unlock(philo->mutexes.right_fork);
		return;
	}
	print_action(philo, " has taken a fork");
	pthread_mutex_lock(philo->mutexes.meal_lock);
	if (*philo->stop)
	{
		pthread_mutex_unlock(philo->mutexes.meal_lock);
		pthread_mutex_unlock(philo->mutexes.left_fork);
		pthread_mutex_unlock(philo->mutexes.right_fork);
		return;
	}
	philo->times.last_meal = get_current_time();
	philo->meals_eaten += 1;
	pthread_mutex_unlock(philo->mutexes.meal_lock);
	print_action(philo, " is eating");
	ft_usleep(philo->times.eat);
	pthread_mutex_unlock(philo->mutexes.left_fork);
	pthread_mutex_unlock(philo->mutexes.right_fork);
	if (is_stopped(philo))
		return;
	print_action(philo, " is sleeping");
	ft_usleep(philo->times.sleep);
	if (!is_stopped(philo))
		print_action(philo, " is thinking");
}

void	*start_simulation(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!is_stopped(philo))
		philo_routine(philo);
	return (NULL);
}

void	launcher(t_engine *engine, int count)
{
	t_id	observer_id;
	int		i;

	if (pthread_create(&observer_id, NULL, obsorver, engine->philos) != 0)
		destroy_all(engine, "[Thread Creation ERROR]\n", count, 1);
	i = -1;
	while (++i < count)
	{
		if (pthread_create(&engine->philos[i].thread_id, NULL,
				start_simulation, &engine->philos[i]) != 0)
		{
			pthread_mutex_lock(&engine->meal_lock);
			engine->stop = true;
			pthread_mutex_unlock(&engine->meal_lock);
			pthread_join(observer_id, NULL);
			while (--i >= 0)
				pthread_join(engine->philos[i].thread_id, NULL);
			destroy_all(engine, "[Thread Creation ERROR]\n", count, 1);
		}
	}
	if (pthread_join(observer_id, NULL) != 0)
		destroy_all(engine, "[Thread Join ERROR]\n", count, 1);
	i = -1;
	while (++i < count)
	{
		if (pthread_join(engine->philos[i].thread_id, NULL) != 0)
			destroy_all(engine, "[Thread Join ERROR]\n", count, 1);
	}
}
