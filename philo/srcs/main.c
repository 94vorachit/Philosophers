/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:37:06 by vorhansa          #+#    #+#             */
/*   Updated: 2026/06/02 18:30:38 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/philo.h"

int check_args(int ac ,char **av)
{
    int     i;
    long    num;
    
    if (ac < 5 || ac > 6)
        return (0);
    i = 0;
    while (i < ac)
    {
        num = ft_atoi(av[i]);
        if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT))
            return (0);
        else if (i == 5 && (num < 0 || num > INT_MAX))
            return (0);
        else if ( i != 1 && i != 5 && (num < 1 || num > INT_MAX))
            return (0);
        
    }
    return (1);
}

int	main(int ac, char **av)
{
	if (!check_args(ac, av))
        error_msg("Argument ERROR\n", 1);
    return (0);
}
