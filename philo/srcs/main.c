/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:37:06 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 20:51:32 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Add the cleanup function that destroys all mutexes and 
 * frees memory after the threads finish.
 */
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

void	print_error(const char *msg)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
}

long long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL);
}

int	main(int ac, char **av)
{
	t_sim	sim;

	if (!parse_args(ac, av, &sim))
	{
		print_error("Error: invalid arguments\n");
		return (1);
	}
	if (!init_sim(&sim))
	{
		print_error("Error: simulation initialization failed\n");
		return (1);
	}
	if (!start_simulation(&sim))
	{
		print_error("Error: could not start simulation\n");
		return (1);
	}
	cleanup_sim(&sim);
	return (0);
}
