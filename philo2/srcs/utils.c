/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 15:00:58 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 15:23:20 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_message(char *text, int signal)
{
	if (text)
		write(2, text, ft_strlen(text));
	exit(signal);
}

void	destroy_all(t_engine *engine, char *str, int count, int signal)
{
	while (--count >= 0)
		pthread_mutex_destroy(&engine->forks[count]);
	pthread_mutex_destroy(&engine->write_lock);
	pthread_mutex_destroy(&engine->meal_lock);
	error_message(str, signal);
}

void	print_action(t_philo *philo, char *action)
{
	size_t	time;
	bool	stopped;
	bool	is_death;

	is_death = action[1] == 'd';
	pthread_mutex_lock(philo->mutexes.meal_lock);
	stopped = *philo->stop;
	pthread_mutex_unlock(philo->mutexes.meal_lock);
	pthread_mutex_lock(philo->mutexes.write_lock);
	if (!stopped || is_death)
	{
		time = get_current_time() - philo->times.born_time;
		printf(GREEN"[%zu]"RESET" %d%s\n", time, philo->id, action);
	}
	pthread_mutex_unlock(philo->mutexes.write_lock);
}

size_t	get_current_time(void)
{
	t_timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (0);
	return ((size_t)time.tv_sec * 1000 + (size_t)time.tv_usec / 1000);
}

void	ft_usleep(size_t mls)
{
	size_t	start;

	start = get_current_time();
	while (get_current_time() - start < mls)
		usleep(500);
}
