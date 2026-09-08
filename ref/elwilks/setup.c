/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 21:13:01 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 15:35:56 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_config(t_config *config, int argc)
{
	if (config->number_of_philosophers <= 0
		|| config->number_of_philosophers > 200
		|| config->time_to_die <= 0
		|| config->time_to_eat <= 0 || config->time_to_sleep <= 0)
		return (0);
	if (argc == 6 && config->amount_to_eat <= 0)
		return (0);
	return (1);
}

int	setup_config(int argc, char **argv, t_config *config)
{
	config->number_of_philosophers = ft_atoi(argv[1]);
	config->time_to_die = ft_atoi(argv[2]);
	config->time_to_eat = ft_atoi(argv[3]);
	config->time_to_sleep = ft_atoi(argv[4]);
	config->amount_to_eat = -1;
	if (argc == 6)
		config->amount_to_eat = ft_atoi(argv[5]);
	config->start_time = get_time_ms();
	config->death_occurred = 0;
	config->forks = NULL;
	config->threads = NULL;
	if (!check_config(config, argc))
		return (printf("Error: invalid argument\n"), 0);
	if (!init_mutexes(config))
		return (printf("Error: cannot initiate mutexes\n"), 0);
	if (!init_fork_mutexes(config))
		return (printf("Error: cannot initiate forks\n"), 0);
	return (1);
}
