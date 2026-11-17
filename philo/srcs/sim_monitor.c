/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_monitor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 19:03:25 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/17 20:06:53 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Purpose: Checks whether every philosopher has finished eating 
 * the required number of meals.
 * 
 * How it works
 * 1. loops over all philosophers
 * 2. locks `state_lock`
 * 3. reads `meals_eaten`
 * 4. if any philosopher has eaten less than `sim->must_eat`, it returns `false`
 * 5. otherwise, it continues
 * 6. after the loop, returns `true`
 * 
 * Why this matters
 * If the optional argument `number_of_times_each_philosopher_must_eat`
 * was provided, the simulation should stop when all philosophers have 
 * reached that count.
 */
bool	all_ate(t_sim *sim)
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

/**
 * Purpose: Handles the death check for one philosopher.
 * 
 * How it works
 * 1. calculates:
 * * `now - sim->philos[index].last_meal_ms`
 * 2. if this is less than or equal to `time_to_die`, it means the philosopher 
 * is still alive enough, so it returns `false`
 * 3. if the philosopher is overdue:
 * * unlocks `state_lock`
 * * locks `print_lock`
 * * locks `stop_lock`
 * * if `sim->stop` is still false, it sets it to true and prints:
 * * * `"<time> <philosopher_id> died"`
 * 4. unlocks the locks
 * 5. returns `true`
 * 
 * Why this matters
 * This is the actual death detection logic. The monitor detects that a 
 * philosopher has not eaten in too long and triggers the end of the simulation.
 */
bool	check_death(t_sim *sim, int index, long long now)
{
	if (now - sim->philos[index].last_meal_ms <= sim->time_to_die)
		return (false);
	pthread_mutex_unlock(&sim->state_lock);
	pthread_mutex_lock(&sim->print_lock);
	pthread_mutex_lock(&sim->stop_lock);
	if (!sim->stop)
	{
		sim->stop = true;
		printf("%lld %d died\n",
			now - sim->start_time_ms, sim->philos[index].id);
	}
	pthread_mutex_unlock(&sim->stop_lock);
	pthread_mutex_unlock(&sim->print_lock);
	return (true);
}

/**
 * Purpose: Runs one full scan of all philosophers to check their status.
 * 
 * How it works
 * 1. loops over every philosopher
 * 2. locks `state_lock`
 * 3. reads the current time
 * 4. if the philosopher has already reached the required meal count, skip them
 * 5. if the time since the last meal is greater than `time_to_die`, 
 * call `check_death(...)`
 * 6. if death is detected, return
 * 7. otherwise unlock the state lock and continue checking the next philosopher
 * 
 * Why this matters
 * This function is the periodic observer that checks the whole table and finds 
 * the starving philosopher.
*/
void	monitor_cycle(t_sim *sim)
{
	int			index;
	long long	now;

	index = 0;
	while (index < sim->philo_count)
	{
		pthread_mutex_lock(&sim->state_lock);
		now = now_ms();
		if (now - sim->philos[index].last_meal_ms > sim->time_to_die)
		{
			if (check_death(sim, index, now))
				return ;
			return ;
		}
		pthread_mutex_unlock(&sim->state_lock);
		index++;
	}
}

/**
 * Purpose: Runs continuously in a dedicated thread to monitor the simulation 
 * while philosophers are eating and sleeping.
 * 
 * How it works
 * 1. receives the simulation pointer
 * 2. loops while !is_stopped(sim)
 * 3. if all philosophers have eaten enough, it calls set_stopped(sim) and exits
 * 4. otherwise it calls monitor_cycle(sim)
 * 5. waits briefly with usleep(500)
 * 
 * Why this matters
 * This thread acts like a watchdog:
 * * it never sleeps for long
 * * it keeps checking
 * * it triggers shutdown when someone dies or everyone is done
 */
void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		monitor_cycle(sim);
		if (sim->must_eat >= 0 && all_ate(sim))
		{
			set_stopped(sim);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}

/**
 * Overall workflow
 * 
 * The logic works like this:
 * 1. Philosophers update last_meal_ms when they eat.
 * 2. The monitor thread continuously checks each philosopher’s time 
 * since last meal.
 * 3. If a philosopher exceeds time_to_die, it prints “died” and 
 * sets the stop flag.
 * 4. If must_eat is set and everyone has eaten enough, 
 * it also stops the simulation.
 * 5. The philosopher threads stop when stop becomes true.
 */