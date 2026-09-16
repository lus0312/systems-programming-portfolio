#include "philo.h"

void	free_forks(t_rules *rules, int i)
{
	while (--i >= 0)
		pthread_mutex_destroy(&rules->forks[i]);
}

void	free_philos(t_philo **philos, int i)
{
	while (--i >= 0)
		pthread_mutex_destroy(&(*philos)[i].meal_mutex);
	free(*philos);
	*philos = NULL;
}

void	free_rules(t_rules *rules)
{
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->dead_mutex);
	pthread_mutex_destroy(&rules->count_mutex);
	if (rules->forks)
	{
		free(rules->forks);
		rules->forks = NULL;
	}
}

void	free_all(t_philo *philos, t_rules *rules)
{
	int	i;

	if (!philos || !rules)
		return ;
	i = 0;
	while (i < philos->rules->nb_philo)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	i = 0;
	while (i < philos->rules->nb_philo)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	free_rules(rules);
	free(philos);
	philos = NULL;
}
