#include "philo.h"

int	check_meal_limit(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo[i].meal_mutex);
	if (philo[i].rules->must_eat != -1
		&& philo[i].eat_count >= philo[i].rules->must_eat)
	{
		if (philo[i].eat_flag != 1)
		{
			pthread_mutex_lock(&philo[i].rules->count_mutex);
			philo->rules->must_eat_count++;
			philo[i].eat_flag = 1;
			if (philo->rules->must_eat_count >= philo[0].rules->nb_philo)
			{
				pthread_mutex_unlock(&philo[i].rules->count_mutex);
				pthread_mutex_lock(&philo[i].rules->dead_mutex);
				philo[i].rules->is_dead = 1;
				pthread_mutex_unlock(&philo[i].rules->dead_mutex);
				pthread_mutex_unlock(&philo[i].meal_mutex);
				return (1);
			}
			pthread_mutex_unlock(&philo[i].rules->count_mutex);
		}
	}
	pthread_mutex_unlock(&philo[i].meal_mutex);
	return (0);
}

int	check_death_or_full(t_philo *philo, int i)
{
	long long	now;

	pthread_mutex_lock(&philo[i].meal_mutex);
	now = get_time_in_ms();
	if (now - philo[i].last_eat >= philo[i].rules->time_to_die)
	{
		pthread_mutex_unlock(&philo[i].meal_mutex);
		pthread_mutex_lock(&philo[i].rules->dead_mutex);
		philo[i].rules->is_dead = 1;
		pthread_mutex_unlock(&philo[i].rules->dead_mutex);
		print_status(&philo[i], "died", 1);
		return (1);
	}
	pthread_mutex_unlock(&philo[i].meal_mutex);
	if (check_meal_limit(philo, i) == 1)
	{
		return (1);
	}
	return (0);
}

int	monitoring(void *arg, t_rules *rules)
{
	t_philo		*philo;
	int			i;

	philo = (t_philo *)arg;
	while (1)
	{
		i = 0;
		while (i < rules->nb_philo)
		{
			if (check_death_or_full(philo, i) == 1)
				return (1);
			i++;
		}
		usleep(50);
	}	
}
