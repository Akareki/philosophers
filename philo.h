/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 13:33:22 by aoizel            #+#    #+#             */
/*   Updated: 2023/12/22 10:10:21 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_fork
{
	int				taken;
	pthread_mutex_t	*lock;
}	t_fork;

typedef struct s_philo
{
	int				nb;
	pthread_t		*thread;
	int				dead;
	unsigned long	time_start;
	t_fork			*left_fork;
	t_fork			*right_fork;
	unsigned long	time_to_die;
	unsigned long	time_to_sleep;
	unsigned long	time_to_eat;
	unsigned long	time_last_ate;
	int				nb_eat;
	pthread_mutex_t	*write_lock;
}	t_philo;

int				ft_atoi(const char *nptr);
int				ft_isdigit(int c);
void			printf_tstamp(char *msg, t_philo *philo, int lock);
void			*philo_life(void *data);
int				init_philosophers(t_philo *philosophers, int nb_philo,
					int argc, char **argv);
int				init_forks(t_fork *forks, int nb_philo);
void			give_forks(t_philo *philosophers, t_fork *forks, int nb_philo);
void			synchronize_time(t_philo *philosophers, int nb_philo);
unsigned int	get_time_ms(void);

#endif
