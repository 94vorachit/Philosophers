/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 16:00:31 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 21:50:13 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// /**
//  * Add a single helper to read the stop flag safely, and use it in a thread 
//  * that exits after a few iterations.
//  * 
//  * it adds safe stop-state checking
//  * it prevents the infinite loop while still using the stop flag
//  * it keeps the thread logic minimal and testable
//  * it is the exact pattern you will reuse in the real philosopher loop later
//  */
// bool	is_stopped(t_sim *sim)
// {
// 	bool	stopped;

// 	pthread_mutex_lock(&sim->stop_lock);
// 	stopped = sim->stop;
// 	pthread_mutex_unlock(&sim->stop_lock);
// 	return (stopped);
// }

// /**
//  * Add one small helper that sets the stop flag after a short finite loop, 
//  * then the philosopher thread exits normally.
//  * 
//  * it avoids the infinite wait
//  * it proves the thread can terminate
//  * it keeps the logic completely separate from eating/death rules
//  * it is the exact “exit condition” pattern the real simulation will need later
//  */
// void	set_stop(t_sim *sim)
// {
// 	pthread_mutex_lock(&sim->stop_lock);
// 	sim->stop = true;
// 	pthread_mutex_unlock(&sim->stop_lock);
// }

// /**
//  * Make the fork-pick order deterministic and safe by 
//  * always taking the lower fork index first.
//  */
// void	lock_forks(t_philo *philo)
// {
// 	int	first;
// 	int	second;

// 	if (philo->left_fork < philo->right_fork)
// 	{
// 		first = philo->left_fork;
// 		second = philo->right_fork;
// 	}
// 	else
// 	{
// 		first = philo->right_fork;
// 		second = philo->left_fork;
// 	}

// 	pthread_mutex_lock(&philo->sim->forks[first]);
// 	log_action(philo, "has taken a fork");
// 	pthread_mutex_lock(&philo->sim->forks[second]);
// 	log_action(philo, "has taken a fork");
// }

// void	unlock_forks(t_philo *philo)
// {
// 	pthread_mutex_unlock(philo->left_lock);
// 	pthread_mutex_unlock(philo->right_lock);
// }

// /**
//  * Add the one missing synchronization rule: a philosopher must not print after death. 
//  * The easiest way is to check the stop flag inside the print helper before printing.
//  */
// void	log_action(t_philo *philo, const char *action)
// {
// 	long long	elapsed;

// 	pthread_mutex_lock(&philo->sim->stop_lock);
// 	if (philo->sim->stop)
// 	{
// 		pthread_mutex_unlock(&philo->sim->stop_lock);
// 		return ;
// 	}
// 	pthread_mutex_unlock(&philo->sim->stop_lock);

// 	pthread_mutex_lock(&philo->sim->print_lock);
// 	pthread_mutex_lock(&philo->sim->stop_lock);
// 	if (!philo->sim->stop)
// 	{
// 		elapsed = now_ms() - philo->sim->start_time_ms;
// 		printf("%lld %d %s\n", elapsed, philo->id, action);
// 	}
// 	pthread_mutex_unlock(&philo->sim->stop_lock);
// 	pthread_mutex_unlock(&philo->sim->print_lock);
// }

// /**
//  * Add the last_meal_ms update right before the philosopher starts eating.
//  */
// void	eat_action(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->sim->state_lock);
// 	philo->last_meal_ms = now_ms();
// 	philo->meals_eaten++;
// 	pthread_mutex_unlock(&philo->sim->state_lock);

// 	log_action(philo, "is eating");
// 	usleep(philo->sim->time_to_eat * 1000);
// }

// /**
//  * Add the completion condition for must_eat in the monitor.
//  */
// int	all_philos_ate(t_sim *sim)
// {
// 	int	i;

// 	pthread_mutex_lock(&sim->state_lock);
// 	i = 0;
// 	while (i < sim->philo_count)
// 	{
// 		if (sim->philos[i].meals_eaten < sim->must_eat)
// 		{
// 			pthread_mutex_unlock(&sim->state_lock);
// 			return (0);
// 		}
// 		i++;
// 	}
// 	pthread_mutex_unlock(&sim->state_lock);
// 	return (1);
// }

// void	handle_death(t_sim *sim, int index)
// {
// 	pthread_mutex_lock(&sim->state_lock);
// 	if (now_ms() - sim->philos[index].last_meal_ms <= sim->time_to_die)
// 	{
// 		pthread_mutex_unlock(&sim->state_lock);
// 		return ;
// 	}
// 	pthread_mutex_unlock(&sim->state_lock);
// 	pthread_mutex_lock(&sim->print_lock);
// 	printf("%lld %d died\n",
// 		now_ms() - sim->start_time_ms, sim->philos[index].id);
// 	pthread_mutex_unlock(&sim->print_lock);
// 	set_stop(sim);
// }

// void	*philo_routine(void *arg)
// {
// 	t_philo	*philo;

// 	philo = (t_philo *)arg;
// 	if (philo->sim->philo_count == 1)
// 	{
// 		pthread_mutex_lock(philo->left_lock);
// 		log_action(philo, "has taken a fork");
// 		usleep(philo->sim->time_to_die * 1000);
// 		pthread_mutex_unlock(philo->left_lock);
// 		set_stop(philo->sim);
// 		pthread_mutex_lock(&philo->sim->print_lock);
// 		printf("%lld %d died\n",
// 			now_ms() - philo->sim->start_time_ms, philo->id);
// 		pthread_mutex_unlock(&philo->sim->print_lock);
// 		return (NULL);
// 	}
// 	while (!is_stopped(philo->sim))
// 	{
// 		lock_forks(philo);
// 		eat_action(philo);
// 		unlock_forks(philo);

// 		if (is_stopped(philo->sim))
// 			return (NULL);

// 		log_action(philo, "is sleeping");
// 		usleep(philo->sim->time_to_sleep * 1000);

// 		if (is_stopped(philo->sim))
// 			return (NULL);

// 		log_action(philo, "is thinking");
// 	}
// 	return (NULL);
// }

// /**
//  * Add the monitor thread before the full death logic, 
//  * with a very small fatal check.
//  * 
//  * This is the first real monitor loop:
//  * * it watches each philosopher
//  * * it transitions the simulation to stop
//  * * it is the foundation for the actual death detection logic
//  */
// void	*monitor_routine(void *arg)
// {
// 	t_sim	*sim;
// 	int		i;

// 	sim = (t_sim *)arg;
// 	while (!is_stopped(sim))
// 	{
// 		if (sim->must_eat > 0 && all_philos_ate(sim))
// 		{
// 			set_stop(sim);
// 			return (NULL);
// 		}
// 		i = 0;
// 		while (i < sim->philo_count)
// 		{
// 			pthread_mutex_lock(&sim->state_lock);
// 			if (now_ms() - sim->philos[i].last_meal_ms > sim->time_to_die)
// 			{
// 				pthread_mutex_unlock(&sim->state_lock);
// 				handle_death(sim, i);
// 				return (NULL);
// 			}
// 			pthread_mutex_unlock(&sim->state_lock);
// 			i++;
// 		}
// 		usleep(1000);
// 	}
// 	return (NULL);
// }

// int	start_simulation(t_sim *sim)
// {
// 	int	index;

// 	sim->threads = malloc(sizeof(pthread_t) * sim->philo_count);
// 	if (!sim->threads)
// 		return (0);
// 	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
// 	{
// 		free(sim->threads);
// 		sim->threads = NULL;
// 		return (0);
// 	}
// 	index = 0;
// 	while (index < sim->philo_count)
// 	{
// 		if (pthread_create(&sim->threads[index], NULL, philo_routine,
// 				&sim->philos[index]) != 0)
// 		{
// 			while (index > 0)
// 			{
// 				pthread_join(sim->threads[--index], NULL);
// 			}
// 			pthread_join(sim->monitor, NULL);
// 			free(sim->threads);
// 			sim->threads = NULL;
// 			return (0);
// 		}
// 		index++;
// 	}
// 	index = 0;
// 	while (index < sim->philo_count)
// 	{
// 		pthread_join(sim->threads[index], NULL);
// 		index++;
// 	}
// 	pthread_join(sim->monitor, NULL);
// 	free(sim->threads);
// 	sim->threads = NULL;
// 	return (1);
// }

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

static void	custom_usleep(long long ms, t_sim *sim)
{
	long long	limit;

	limit = now_ms() + ms;
	while (now_ms() < limit && !is_stopped(sim))
		usleep(500);
}

static void	lock_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_lock);
	log_action(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_lock);
	log_action(philo, "has taken a fork");
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
	if (philo->id % 2 == 0)
		custom_usleep(1, philo->sim);
	if (philo->sim->philo_count == 1)
	{
		pthread_mutex_lock(philo->left_lock);
		log_action(philo, "has taken a fork");
		custom_usleep(philo->sim->time_to_die, philo->sim);
		pthread_mutex_unlock(philo->left_lock);
		if (!is_stopped(philo->sim))
		{
			set_stopped(philo->sim);
			pthread_mutex_lock(&philo->sim->print_lock);
			printf("%lld %d died\n",
				now_ms() - philo->sim->start_time_ms, philo->id);
			pthread_mutex_unlock(&philo->sim->print_lock);
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

static bool	all_ate(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->philo_count)
	{
		pthread_mutex_lock(&sim->state_lock);
		if (sim->philos[index].meals_eaten < sim->must_eat)
		{
			pthread_mutex_unlock(&sim->state_lock);
			return (false);
		}
		pthread_mutex_unlock(&sim->state_lock);
		index++;
	}
	return (true);
}

static void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		index;
	long long	now;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		if (sim->must_eat >= 0 && all_ate(sim))
		{
			set_stopped(sim);
			return (NULL);
		}
		index = 0;
		while (index < sim->philo_count)
		{
			pthread_mutex_lock(&sim->state_lock);
			now = now_ms();
			if (sim->must_eat >= 0
				&& sim->philos[index].meals_eaten >= sim->must_eat)
			{
				pthread_mutex_unlock(&sim->state_lock);
				index++;
				continue ;
			}
			if (now - sim->philos[index].last_meal_ms > sim->time_to_die)
			{
				pthread_mutex_unlock(&sim->state_lock);
				set_stopped(sim);
				pthread_mutex_lock(&sim->print_lock);
				printf("%lld %d died\n",
					now - sim->start_time_ms, sim->philos[index].id);
				pthread_mutex_unlock(&sim->print_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&sim->state_lock);
			index++;
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
