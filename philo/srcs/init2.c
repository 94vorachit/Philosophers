/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 17:36:16 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/17 15:18:52 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Purpose: This destroys all mutexes that were created during setup, 
 * so there are no leaked thread locks when the simulation ends.
 * 
 * What it does
 * 1. It starts by checking each initialization flag and If a flag is true, 
 * it calls:
 * * `pthread_mutex_destroy(&sim->print_lock);`
 * * `pthread_mutex_destroy(&sim->state_lock);`
 * * `pthread_mutex_destroy(&sim->stop_lock);`
 * 2. Destroys the fork mutexes: 
 * * loop while `index < sim->forks_init_count`
 * * call `pthread_mutex_destroy(&sim->forks[index]);`
 * This is important because each fork has its own mutex, 
 * and they must all be cleaned up properly.
 * 3. After that:
 * * `free(sim->philos);`
 * * `free(sim->forks);`
 * And it sets both pointers to NULL so no dangling pointer remains.
 * 
 * Without this function, the program would leak mutexes and memory, 
 * and may produce undefined behavior or thread cleanup failures.
 */
void	destroy_mutexes(t_sim *sim)
{
	int	index;

	if (sim->print_lock_init)
		pthread_mutex_destroy(&sim->print_lock);
	if (sim->state_lock_init)
		pthread_mutex_destroy(&sim->state_lock);
	if (sim->stop_lock_init)
		pthread_mutex_destroy(&sim->stop_lock);
	index = 0;
	while (index < sim->forks_init_count)
	{
		pthread_mutex_destroy(&sim->forks[index]);
		index++;
	}
	free(sim->philos);
	sim->philos = NULL;
	free(sim->forks);
	sim->forks = NULL;
}

/**
 * Purpose: This allocates the memory for the philosopher array 
 * and the fork array.
 * 
 * What it does
 * 1. `sim->philos = malloc(sizeof(t_philo) * sim->philo_count);`
 * * an array of philosopher structs is created
 * 2. `sim->forks = malloc(sizeof(pthread_mutex_t) * sim->philo_count);`
 * * an array of mutexes is created, one for each fork
 * 3. if either allocation fails, it frees both and returns `0`
 * 4. otherwise, it returns `1`
 * 
 * Later, each philosopher needs to be stored in `sim->philos[]`, 
 * and each fork needs a mutex in `sim->forks[]`.
 * 
 * Without allocation, there would be no place to store the philosophers 
 * or lock the forks.
 */
int	alloc_sim(t_sim *sim)
{
	sim->philos = malloc(sizeof(t_philo) * sim->philo_count);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->philo_count);
	if (!sim->philos || !sim->forks)
	{
		free(sim->philos);
		sim->philos = NULL;
		free(sim->forks);
		sim->forks = NULL;
		return (0);
	}
	return (1);
}

/**
 * The usual flow is:
 * 1. `alloc_sim(sim)`
 * * create memory for philosopher list and fork mutexes
 * 2. initialize the shared locks
 * 3. initialize each fork mutex
 * 4. initialize each philosopher using init_philo(...)
 * 5. run the simulation
 * 6. call `destroy_mutexes(sim)` to clean everything up
 */