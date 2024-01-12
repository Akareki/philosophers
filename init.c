/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 15:17:48 by aoizel            #+#    #+#             */
/*   Updated: 2023/12/22 10:36:28 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philosophers(t_philo *philosophers, int nb_philo,
					int argc, char **argv)
{
	int				i;
	pthread_mutex_t	*write_lock;

	if (!philosophers)
		return (-1);
	i = 0;
	write_lock = malloc(sizeof(pthread_mutex_t));
	if (!write_lock)
		return (free(philosophers), -1);
	if (pthread_mutex_init(write_lock, NULL))
		return (free(philosophers), free(write_lock), -1);
	while (i < nb_philo)
	{
		philosophers[i].nb = i;
		philosophers[i].time_to_die = ft_atoi(argv[2]);
		philosophers[i].time_to_eat = ft_atoi(argv[3]);
		philosophers[i].time_to_sleep = ft_atoi(argv[4]);
		philosophers[i].nb_eat = -1 + (argc == 6) * (ft_atoi(argv[5]) + 1);
		philosophers[i].dead = 0;
		philosophers[i].write_lock = write_lock;
		philosophers[i].thread = NULL;
		i++;
	}
	return (0);
}

int	init_forks(t_fork *forks, int nb_philo)
{
	int	i;
	int	err;

	i = 0;
	while (i < nb_philo)
	{
		err = 1;
		forks[i].taken = 0;
		forks[i].lock = malloc(sizeof(pthread_mutex_t));
		if (forks[i].lock)
			err = pthread_mutex_init(forks[i].lock, NULL);
		if (err != 0)
		{
			free(forks[i].lock);
			while (--i >= 0)
			{
				pthread_mutex_destroy(forks[i].lock);
				free(forks[i].lock);
			}
			return (-1);
		}
		i++;
	}
	return (0);
}

void	give_forks(t_philo *philosophers, t_fork *forks, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		philosophers[i].left_fork = forks + i;
		philosophers[i].right_fork = forks + ((i + 1) % nb_philo);
		i++;
	}
}

void	synchronize_time(t_philo *philosophers, int nb_philo)
{
	unsigned long	time_start;
	int				i;

	i = 0;
	time_start = get_time_ms();
	while (i < nb_philo)
	{
		philosophers[i].time_start = time_start;
		philosophers[i].time_last_ate = time_start;
		i++;
	}
}
