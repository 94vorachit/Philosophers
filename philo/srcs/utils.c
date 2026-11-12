/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:44:50 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 15:41:11 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

long	ft_atoi(const char *str)
{
	long	num;
	int		sign;
	int		i;

	num = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num = num + (str[i] - '0');
		i++;
		if (num > INT_MAX)
			break ;
	}
	return (num * sign);
}

int	is_number(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	get_int_value(const char *s, int *out)
{
	long	value;

	if (!is_number(s))
		return (0);
	value = ft_atoi(s);
	if (value < INT_MIN || value > INT_MAX)
		return (0);
	*out = (int)value;
	return (1);
}

int	parse_args(int ac, char **av, t_sim *sim)
{
	if (ac < 5 || ac > 6)
		return (0);
	if (!get_int_value(av[1], &sim->philo_count))
		return (0);
	if (!get_int_value(av[2], &sim->time_to_die))
		return (0);
	if (!get_int_value(av[3], &sim->time_to_eat))
		return (0);
	if (!get_int_value(av[4], &sim->time_to_sleep))
		return (0);
	if (sim->philo_count < 1 || sim->philo_count > PHILO_MAX_COUNT)
		return (0);
	if (sim->time_to_die <= 0 || sim->time_to_eat <= 0
		|| sim->time_to_sleep <= 0)
		return (0);
	sim->must_eat = -1;
	if (ac == 6)
	{
		if (!get_int_value(av[5], &sim->must_eat))
			return (0);
		if (sim->must_eat < 0)
			return (0);
	}
	return (1);
}
