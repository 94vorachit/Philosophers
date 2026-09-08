/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 14:21:59 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 16:14:35 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *nptr)
{
	long	num;
	int		i;

	i = 0;
	if (nptr[i] == '+')
		i++;
	num = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = (num * 10) + (nptr[i++] - '0');
		if (num > INT_MAX)
			return (-1);
	}
	return ((int)num);
}

static int	validate_string(char *str)
{
	int	j;

	j = -1;
	while (str[++j])
	{
		if ((str[j] < '0' || str[j] > '9') && str[j] != '+')
			return (0);
	}
	return (1);
}

int	check_arguments(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (0);
	}
	i = 0;
	while (++i < argc)
	{
		if (!validate_string(argv[i]))
		{
			printf("Error: Invalid argument\n");
			return (0);
		}
		if (ft_atoi(argv[i]) <= 0)
		{
			printf("Error: Positive numbers less than 2147483647 only\n");
			return (0);
		}
	}
	return (1);
}
