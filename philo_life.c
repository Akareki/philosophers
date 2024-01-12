/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 15:15:09 by aoizel            #+#    #+#             */
/*   Updated: 2023/12/22 10:08:44 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_think(t_philo *philo, int *left_fork, int *right_fork)
{
	printf_tstamp("is thinking", philo, 1);
	while (*left_fork == 0 || *right_fork == 0)
	{
		pthread_mutex_lock(philo->right_fork->lock);
		if (!philo->right_fork->taken)
		{
			*right_fork = 1;
			philo->right_fork->taken = 1;
			printf_tstamp("has taken a fork", philo, 1);
		}
		pthread_mutex_unlock(philo->right_fork->lock);
		pthread_mutex_lock(philo->left_fork->lock);
		if (!philo->left_fork->taken)
		{
			*left_fork = 1;
			philo->left_fork->taken = 1;
			printf_tstamp("has taken a fork", philo, 1);
		}
		pthread_mutex_unlock(philo->left_fork->lock);
		usleep(5);
		if (get_time_ms() - philo->time_last_ate > philo->time_to_die)
			return (-1);
	}
	return (0);
}

int	philo_eat(t_philo *philo, int *right_fork, int *left_fork)
{
	philo->time_last_ate = get_time_ms();
	printf_tstamp("is eating", philo, 1);
	pthread_mutex_lock(philo->write_lock);
	if (philo->nb_eat > 0)
		philo->nb_eat--;
	pthread_mutex_unlock(philo->write_lock);
	while (get_time_ms() - philo->time_last_ate < philo->time_to_eat)
	{
		usleep(500);
		if (get_time_ms() - philo->time_last_ate > philo->time_to_die)
			return (-1);
	}
	pthread_mutex_lock(philo->right_fork->lock);
	philo->right_fork->taken = 0;
	*right_fork = 0;
	pthread_mutex_unlock(philo->right_fork->lock);
	pthread_mutex_lock(philo->left_fork->lock);
	philo->left_fork->taken = 0;
	*left_fork = 0;
	pthread_mutex_unlock(philo->left_fork->lock);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	unsigned long	time_start_sleep;

	time_start_sleep = get_time_ms();
	printf_tstamp("is sleeping", philo, 1);
	while (get_time_ms() - time_start_sleep < philo->time_to_sleep)
	{
		usleep(500);
		if (get_time_ms() - philo->time_last_ate > philo->time_to_die)
			return (-1);
	}
	return (0);
}

void	*philo_death(t_philo *philo)
{
	pthread_mutex_lock(philo->write_lock);
	philo->dead = 1;
	pthread_mutex_unlock(philo->write_lock);
	return (NULL);
}

void	*philo_life(void *data)
{
	t_philo	*philo;
	int		left_fork;
	int		right_fork;

	philo = (t_philo *)data;
	left_fork = 0;
	right_fork = 0;
	usleep(1000 * (philo->nb % 2));
	pthread_mutex_lock(philo->write_lock);
	while (1 && !philo->dead)
	{
		pthread_mutex_unlock(philo->write_lock);
		if (philo_think(philo, &left_fork, &right_fork) == -1)
			return (philo_death(philo));
		if (philo_eat(philo, &right_fork, &left_fork) == -1)
			return (philo_death(philo));
		if (philo_sleep(philo) == -1)
			return (philo_death(philo));
		pthread_mutex_lock(philo->write_lock);
	}
	pthread_mutex_unlock(philo->write_lock);
	return (NULL);
}
