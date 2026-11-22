/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 15:00:33 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 15:23:20 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_args(int argc, char **argv)
{
	int		i;
	long	num;

	if (argc < 5 || argc > 6)
		error_message("[Argument Count ERROR]\n", 1);
	i = 0;
	while (++i < argc)
	{
		num = ft_atoi(argv[i]);
		if (num < 0)
			error_message("[Argument ERROR]\n", 1);
		if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT))
			error_message("[Argument ERROR]\n", 1);
		if (i != 1 && i != 5 && num < 1)
			error_message("[Argument ERROR]\n", 1);
		if (i == 5 && num == 0)
			error_message("[Argument ERROR]\n", 1);
	}
}

int	main(int argc, char **argv)
{
	t_philo		philos[PHILO_MAX_COUNT];
	t_mutex		forks[PHILO_MAX_COUNT];
	t_engine	engine;

	check_args(argc, argv);
	init_engine(&engine, philos, forks);
	init_forks(&engine, forks, ft_atoi(argv[1]));
	init_philos(&engine, philos, forks, argv);
	launcher(&engine, philos[0].philo_count);
	destroy_all(&engine, NULL, philos[0].philo_count, 0);
	return (0);
}
