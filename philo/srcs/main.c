/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:37:06 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 17:31:48 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Purpose: This function destroys the shared control mutexes.
 * 
 * It does:
 * * if `sim->print_lock_init` is true, destroy `print_lock`
 * * if `sim->state_lock_init` is true, destroy `state_lock`
 * * if `sim->stop_lock_init` is true, destroy `stop_lock`
 * * if `sim->turn_lock_init` is true, destroy `turn_lock`
 * Then it resets the flags:
 * * `print_lock_init = false`
 * * `state_lock_init = false`
 * * `stop_lock_init = false`
 * * `turn_lock_init = false`
 * * `forks_init_count = 0`
 * 
 * Destroying them prevents resource leaks.
 */
void	destroy_control_locks(t_sim *sim)
{
	if (sim->print_lock_init)
		pthread_mutex_destroy(&sim->print_lock);
	if (sim->state_lock_init)
		pthread_mutex_destroy(&sim->state_lock);
	if (sim->stop_lock_init)
		pthread_mutex_destroy(&sim->stop_lock);
	if (sim->turn_lock_init)
		pthread_mutex_destroy(&sim->turn_lock);
	sim->print_lock_init = false;
	sim->state_lock_init = false;
	sim->stop_lock_init = false;
	sim->turn_lock_init = false;
	sim->forks_init_count = 0;
}

/**
 * Purpose: This function makes sure all dynamically allocated memory 
 * and mutexes are properly released before exiting.
 * 
 * It does:
 * 1. if `sim->threads` exists, free it
 * 2. if `sim->philos` exists, free it
 * 3. loop over `sim->forks` up to `sim->forks_init_count`
 * 4. destroy each fork mutex
 * 5. free `sim->forks`
 * 6. call `destroy_control_locks(sim)`
 * 
 * If not cleaned up, the program may leak memory or 
 * leave locks in an invalid state.
 */
void	cleanup_sim(t_sim *sim)
{
	int	index;

	if (sim->threads)
	{
		free(sim->threads);
		sim->threads = NULL;
	}
	if (sim->philos)
	{
		free(sim->philos);
		sim->philos = NULL;
	}
	index = 0;
	while (index < sim->forks_init_count)
	{
		pthread_mutex_destroy(&sim->forks[index]);
		index++;
	}
	if (sim->forks)
	{
		free(sim->forks);
		sim->forks = NULL;
	}
	destroy_control_locks(sim);
}

/**
 * Purpose: This prints an error message to standard error.
 * 
 * It checks:
 * 1. if `msg` is not `NULL`
 * 2. then write the string to file descriptor `2`
 */
void	print_error(const char *msg)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
}

/**
 * This returns the current time in milliseconds.
 * 
 * It uses:
 * 1. gettimeofday(...)
 * 2. tv.tv_sec
 * 3. tv.tv_usec
 * 
 * Then it converts it to:
 * 1. seconds * 1000
 * 2. microseconds / 1000
 * 
 * So the final value is a millisecond timestamp.
 */
long long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL);
}

/**
 * This is the main function, the program entry point.
 * 
 * Flow:
 * 1. memset(&sim, 0, sizeof(sim));
 * * zero-initialize the simulation struct
 * 2. if (!parse_args(ac, av, &sim))
 * * parse command-line arguments
 * * if invalid, print error and return 1
 * 3. if (!init_sim(&sim))
 * * initialize philosophers, mutexes, and forks
 * * if it fails, print message and return 1
 * 4. if (!start_simulation(&sim))
 * * start all philosopher threads
 * * if start fails, call cleanup_sim and return 1
 * 5. after successful simulation:
 * * call cleanup_sim(&sim)
 * * return 0
 */
int	main(int ac, char **av)
{
	t_sim	sim;

	memset(&sim, 0, sizeof(sim));
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
		cleanup_sim(&sim);
		return (1);
	}
	cleanup_sim(&sim);
	return (0);
}

/**
 * This file is the control center of the philosophers program:
 * 
 * it validates input, initializes the simulation, starts the threads, 
 * and cleans everything up safely afterward.
 */