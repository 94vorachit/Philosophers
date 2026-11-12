/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:37:06 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 18:39:37 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Add the cleanup function that destroys all mutexes and 
 * frees memory after the threads finish.
 */
void	cleanup_sim(t_sim *sim)
{
    int	i;

    i = 0;
    while (i < sim->philo_count)
    {
        pthread_mutex_destroy(&sim->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&sim->print_lock);
    pthread_mutex_destroy(&sim->state_lock);
    pthread_mutex_destroy(&sim->stop_lock);
    free(sim->forks);
    free(sim->philos);
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
