/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoizel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 13:35:27 by aoizel            #+#    #+#             */
/*   Updated: 2023/12/22 10:08:50 by aoizel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	printf_tstamp(char *msg, t_philo *philo, int lock)
{
	if (lock)
	{
		pthread_mutex_lock(philo->write_lock);
		printf("%08ld %d %s\n", get_time_ms() - philo->time_start,
			philo->nb, msg);
		pthread_mutex_unlock(philo->write_lock);
	}
	else
		printf("%08ld %d %s\n", get_time_ms() - philo->time_start,
			philo->nb, msg);
}

unsigned int	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_atoi(const char *nptr)
{
	int	num;
	int	i;

	num = 0;
	i = 0;
	if (!nptr)
		return (-1);
	while (nptr[i])
	{
		if (!ft_isdigit(nptr[i]))
			return (-1);
		if ((long)num * (long)(10 * num + nptr[i] - '0') < 0)
			return (-1);
		num = 10 * num + nptr[i] - '0';
		i++;
	}
	return (num);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
