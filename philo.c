/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 13:24:20 by aoizel            #+#    #+#             */
/*   Updated: 2023/12/22 10:22:31 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_args(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
		return (-1);
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 0)
			return (-1);
		i++;
	}
	return (0);
}

void	free_all(t_philo *philosophers, t_fork *forks, int nb_philo, int start)
{
	int	i;

	i = 0;
	while (i < start)
	{
		pthread_mutex_lock(philosophers->write_lock);
		philosophers[i].dead = 1;
		pthread_mutex_unlock(philosophers->write_lock);
		i++;
	}
	i = 0;
	while (i < start)
		pthread_join(*(philosophers[i++].thread), NULL);
	i = 0;
	while (i < nb_philo)
	{
		pthread_mutex_destroy(philosophers[i].left_fork->lock);
		free(philosophers[i].left_fork->lock);
		free(philosophers[i].thread);
		i++;
	}
	pthread_mutex_destroy(philosophers->write_lock);
	free(philosophers->write_lock);
	free(philosophers);
	free(forks);
}

int	start_threads(t_philo *philosophers, t_fork *forks, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		philosophers[i].thread = malloc(sizeof(pthread_t));
		if (!philosophers[i].thread)
		{
			free_all(philosophers, forks, nb_philo, 0);
			return (-1);
		}
		i++;
	}
	i = 0;
	synchronize_time(philosophers, nb_philo);
	while (i < nb_philo)
	{
		if (pthread_create(philosophers[i].thread, NULL,
				philo_life, philosophers + i) != 0)
			return (free_all(philosophers, forks, nb_philo, i), -1);
		i++;
	}
	return (0);
}

void	watch_philo(t_philo *philosophers, int nb_philo)
{
	int	i;
	int	full_philosophers;

	while (1)
	{
		i = 0;
		full_philosophers = 0;
		pthread_mutex_lock(philosophers->write_lock);
		while (i < nb_philo)
		{
			if (philosophers[i].dead == 1)
			{
				printf_tstamp("died", philosophers + i, 0);
				pthread_mutex_unlock(philosophers->write_lock);
				return ;
			}
			if (philosophers[i].nb_eat == 0)
				full_philosophers++;
			i++;
		}
		pthread_mutex_unlock(philosophers->write_lock);
		if (full_philosophers == nb_philo)
			return ;
		usleep(100);
	}
}

int	main(int argc, char **argv)
{
	t_philo	*philosophers;
	t_fork	*forks;
	int		nb_philo;

	if (check_args(argc, argv) == -1)
		return (-1);
	nb_philo = ft_atoi(argv[1]);
	philosophers = malloc(nb_philo * sizeof(t_philo));
	if (init_philosophers(philosophers, nb_philo, argc, argv) == -1)
		return (-1);
	forks = malloc(nb_philo * sizeof(t_fork));
	if (!forks || init_forks(forks, nb_philo) == -1)
	{
		free_all(philosophers, forks, -1, 0);
		return (-1);
	}
	give_forks(philosophers, forks, nb_philo);
	if (start_threads(philosophers, forks, nb_philo) == -1)
		return (-1);
	watch_philo(philosophers, nb_philo);
	free_all(philosophers, forks, nb_philo, nb_philo);
}
