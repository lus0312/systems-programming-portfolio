#include "philo.h"

void handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork", 0);
	precise_usleep(philo->rules->time_to_die, philo);
	pthread_mutex_unlock(philo->left_fork);
	print_status(philo, "died", 1);
}

static int stop_started_threads(t_rules *rules, t_philo *philo, int count)
{
	rules->is_dead = 1;
	pthread_mutex_unlock(&rules->dead_mutex);
	while (count > 0)
		pthread_join(philo[--count].thread, NULL);
	write(2, "thread create error\n", 20);
	return (1);
}

int start_simulation(t_rules *rules, t_philo *philo)
{
	int i;

	if (rules->nb_philo == 1)
		return (handle_one_philo(philo), 0);
	/* Workers wait in check_stop until every thread has been created. */
	pthread_mutex_lock(&rules->dead_mutex);
	i = 0;
	while (i < rules->nb_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, philo_routine,
				&philo[i]) != 0)
			return (stop_started_threads(rules, philo, i));
		i++;
	}
	rules->start_time = get_time_in_ms();
	i = -1;
	while (++i < rules->nb_philo)
		philo[i].last_eat = rules->start_time;
	pthread_mutex_unlock(&rules->dead_mutex);
	monitoring(philo, rules);
	return (0);
}

int main(int ac, char **av)
{
	t_rules rules;
	t_philo *philo;
	int i;
	int status;

	if (init_rules(&rules, ac, av) != 0)
	{
		write(2, "Error: invalid arguments\n", 25);
		return (1);
	}
	if (init_mutexes(&rules) != 0)
		return (1);
	rules.start_time = get_time_in_ms();
	if (init_philo(&rules, &philo) != 0
		|| init_philo_mutex(&rules, philo) != 0)
		return (1);
	status = start_simulation(&rules, philo);
	i = -1;
	if (status == 0 && rules.nb_philo > 1)
		while (++i < rules.nb_philo)
			pthread_join(philo[i].thread, NULL);
	free_all(philo, &rules);
	return (status);
}
