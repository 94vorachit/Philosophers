/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:25:14 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/29 21:11:50 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_positive_number(char *str)
{
	int	i;

	if (str == NULL || !*str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long	atoi_positive(char *str)
{
	long	nbr;
	int		i;
	int		digit;

	nbr = 0;
	i = 0;
	while (str[i])
	{
		digit = str[i] - '0';
		if (nbr > (LONG_MAX - digit) / 10)
			return (-1);
		nbr = nbr * 10 + digit;
		i++;
	}
	return (nbr);
}

void	fill_config(int *args, t_config *config)
{
	config->number_of_philosophers = (int)args[0];
	config->time_to_die = args[1];
	config->time_to_eat = args[2];
	config->time_to_sleep = args[3];
	config->amount_to_eat = 0;
}

int	parse_args(int argc, char **argv, t_config *config)
{
	int		i;
	int		args[5];

	if (argc != 5 && argc != 6)
		return (printf("Incorrect Number of Arguments\n"), 0);
	i = 1;
	while (i < argc)
	{
		if (is_positive_number(argv[i]) == 0)
			return (printf("Only Positive Numbers\n"), 0);
		args[i - 1] = atoi_positive(argv[i]);
		if (args[i - 1] < 0)
			return (printf("Error: Number Too Big\n"), 0);
		i++;
	}
	if (args[0] < 1 || args[0] > 200)
		return (printf("Error: No of Philo must be between 1 and 200\n"), 0);
	fill_config(args, config);
	if (argc == 6)
		config->amount_to_eat = args[4];
	else
		config->amount_to_eat = -1;
	return (1);
}
