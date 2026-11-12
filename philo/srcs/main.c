/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:37:06 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 15:40:47 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(const char *msg)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
}

long long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL);
}

int	main(int ac, char **av)
{
	t_sim	sim;

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
	return (0);
}
