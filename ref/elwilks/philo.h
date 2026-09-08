/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:25:18 by elwilks           #+#    #+#             */
/*   Updated: 2026/08/31 15:35:36 by elwilks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*::::::::::::::::::::   LIBRARIES   :::::::::::::::::::::*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

/*::::::::::::::::::::   STRUCTS    :::::::::::::::::::::*/

typedef struct s_config	t_config;

typedef struct s_philo
{
	int				id;
	long			last_meal_time;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_config		*config;
}	t_philo;

typedef struct s_config
{
	int				number_of_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				amount_to_eat;
	long long		start_time;
	int				death_occurred;

	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	meal_mutex;

	pthread_t		*threads;
	pthread_t		monitor;
	t_philo			*philos;
}	t_config;

/*::::::::::::::::::::   FUNCTIONS    :::::::::::::::::::::*/

int		init_config(t_config *config);
int		init_philos(t_config *config);
int		init_mutexes(t_config *config);
int		init_fork_mutexes(t_config *config);

void	print_death(t_philo *philo);
int		parse_args(int argc, char **argv, t_config *config);
void	*routine(void *arg);
long	get_time_ms(void);
void	*monitor(void *config);
void	cleanup(t_config *config);
int		fork_assign(pthread_mutex_t **f, pthread_mutex_t **s, t_philo *philo);
int		setup_config(int argc, char **argv, t_config *config);
int		check_arguments(int argc, char **argv);
int		ft_atoi(const char *nptr);
void	print_status(t_philo *philo, char *str);
void	ft_usleep(int ms);
void	simple_usleep(size_t ms);
int		died(t_philo *philo);
void	check_all_ate(t_philo *philo);
void	*one_philo_routine(t_philo *philo);
int		eaten_enough(t_philo *philo);
void	think(t_philo *philo);
void	philo_sleep(t_philo *philo);

#endif //PHILO_H
