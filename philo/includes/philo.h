/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:43:45 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/12 19:21:06 by vorhansa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define PHILO_MAX_COUNT 200

typedef struct s_sim t_sim;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	long long		last_meal_ms;
	int				meals_eaten;
	pthread_mutex_t	*left_lock;
	pthread_mutex_t	*right_lock;
	t_sim			*sim;
}	t_philo;

struct s_sim
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	long long		start_time_ms;
	bool			stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	state_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;

	pthread_t		*threads;
	pthread_t		monitor;
};

void		print_error(const char *msg);
long long	now_ms(void);

size_t		ft_strlen(const char *s);
long		ft_atoi(const char *str);
int			parse_args(int ac, char **av, t_sim *sim);

int			init_sim(t_sim *sim);void	destroy_mutexes(t_sim *sim);
int			start_simulation(t_sim *sim);
void		log_action(t_philo *philo, const char *action);

#endif