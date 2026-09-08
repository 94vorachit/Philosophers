/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 21:09:45 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 16:19:48 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_config *config)
{
	int			i;

	config->threads = malloc(sizeof(pthread_t)
			* config->number_of_philosophers);
	if (config->threads == NULL)
		return (0);
	i = 0;
	while (i < config->number_of_philosophers)
	{
		if (pthread_create(&config->threads[i],
				NULL, &routine, &config->philos[i]))
			return (0);
		i++;
	}
	if (pthread_create(&config->monitor, NULL, &monitor, config))
		return (0);
	return (1);
}

void	join_threads(t_config *config)
{
	int		i;

	i = 0;
	while (i < config->number_of_philosophers)
	{
		pthread_join(config->threads[i], NULL);
		i++;
	}
	pthread_join(config->monitor, NULL);
}

int	main(int argc, char **argv)
{
	t_config	config;

	if (!check_arguments(argc, argv))
		return (1);
	if (!setup_config(argc, argv, &config))
		return (cleanup(&config), 1);
	if (init_philos(&config) == 0)
		return (cleanup(&config), 1);
	if (!create_threads(&config))
		return (cleanup(&config), 1);
	join_threads(&config);
	cleanup(&config);
	return (0);
}
