/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 15:00:50 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 15:23:20 by vorhansa         ###   ########.fr       */
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

long	ft_atoi(const char *str)
{
	long	num;
	int		i;

	if (!str || !str[0])
		return (-1);
	i = 0;
	if (str[i] == '+')
		++i;
	if (!str[i])
		return (-1);
	num = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		if (num > (INT_MAX - (str[i] - '0')) / 10)
			return (-1);
		num = num * 10 + (str[i] - '0');
		++i;
	}
	return (num);
}
