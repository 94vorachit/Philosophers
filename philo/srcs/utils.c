/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:44:50 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 14:49:18 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

long	ft_strtol(const char *str, char **endptr)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (result > (LONG_MAX - (*str - '0')) / 10)
		{
			errno = ERANGE;
			if (sign > 0)
				return (LONG_MAX);
			return (LONG_MIN);
		}
		result = result * 10 + (*str - '0');
		str++;
	}
	if (endptr)
		*endptr = (char *)str;
	if (sign < 0)
		return (-result);
	return (result);
}

static int	is_number(const char *s)
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

static int	get_int_value(const char *s, int *out)
{
    char	*end;
    long	value;

    if (!is_number(s))
        return (0);
    errno = 0;
    value = ft_strtol(s, &end);
    if (errno != 0 || *end != '\0')
        return (0);
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
    if (sim->time_to_die <= 0 || sim->time_to_eat <= 0 || sim->time_to_sleep <= 0)
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