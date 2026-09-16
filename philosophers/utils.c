/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:21:43 by eulee             #+#    #+#             */
/*   Updated: 2025/08/24 19:31:25 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (*str && *str != '\0')
	{
		len++;
		str++;
	}
	return (len);
}

int ft_atoi(const char *str)
{
	int result;
	int digit;

	result = 0;
	if (*str == '+')
		str++;
	if (*str == '\0')
		return (-1);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		digit = *str - '0';
		if (result > (INT_MAX - digit) / 10)
			return (-1);
		result = result * 10 + digit;
		str++;
	}
	return (result);
}

long long	get_time_in_ms(void)
{
	struct timeval	now;

	if (gettimeofday(&now, NULL) != 0)
		return (-1);
	return ((long long)now.tv_sec * 1000 + now.tv_usec / 1000);
}

void	print_status(t_philo *philo, char *msg, int force)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->rules->print_mutex);
	pthread_mutex_lock(&philo->rules->dead_mutex);
	if (philo->rules->is_dead && !force)
	{
		pthread_mutex_unlock(&philo->rules->dead_mutex);
		pthread_mutex_unlock(&philo->rules->print_mutex);
		return ;
	}
	timestamp = get_time_in_ms() - philo->rules->start_time;
	if (!(philo->rules->is_dead && !force))
		printf("%lld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->rules->dead_mutex);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

int	check_stop(void *arg)
{
	t_philo	*philo;
	int		is_dead;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->rules->dead_mutex);
	is_dead = philo->rules->is_dead;
	pthread_mutex_unlock(&philo->rules->dead_mutex);
	return (is_dead);
}
