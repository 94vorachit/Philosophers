/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vorhansa <vorhansa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:43:45 by vorhansa          #+#    #+#             */
/*   Updated: 2026/11/22 21:25:11 by vorhansa         ###   ########.fr       */
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

typedef struct s_sim	t_sim;

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
	bool			print_lock_init;
	bool			state_lock_init;
	bool			stop_lock_init;
	int				forks_init_count;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	state_lock;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	pthread_t		monitor;
	t_philo			*philos;
};

/*init*/
int			init_sim(t_sim *sim);
/*init2*/
void		destroy_mutexes(t_sim *sim);
int			alloc_sim(t_sim *sim);
/*main*/
long long	now_ms(void);
/*utils*/
size_t		ft_strlen(const char *s);
long		ft_atoi(const char *str);
int			parse_args(int ac, char **av, t_sim *sim);
/*simulation*/
int			start_simulation(t_sim *sim);
/*sim_utils*/
bool		is_stopped(t_sim *sim);
void		set_stopped(t_sim *sim);
void		custom_usleep(long long ms, t_sim *sim);
void		unlock_forks(t_philo *philo);
/*sim_utils2*/
void		lock_forks(t_philo *philo);
/*sim_action*/
void		log_action(t_philo *philo, const char *msg);
void		*philo_routine(void *arg);
/*sim_monitor*/
void		*monitor_routine(void *arg);

#endif