/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asideris <asideris@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:41:20 by asideris          #+#    #+#             */
/*   Updated: 2024/08/14 15:37:16 by asideris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	get_current_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
int	ft_set_state(t_philosopher *philo)
{
	if (philo->state == 'e')
	{
		print_status(philo->id, philo->data, "is eating");
		ft_usleep(philo->data->t_to_eat);
		philo->eat_count++;
		if (philo->eat_count == philo->data->min_meals)
			philo->data->finished_philosphers++;
		pthread_mutex_unlock(philo->next_fork);
		pthread_mutex_unlock(&philo->own_fork);
		philo->state = 's';
		print_status(philo->id, philo->data, "is sleeping");
		ft_usleep(philo->data->t_to_sleep);
		philo->state = 't';
		print_status(philo->id, philo->data, "is thinking");
	}
	return (1);
}
void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(&philo->data->starting_block);
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (philo->data->finished_philosphers >= philo->data->philo_c
			|| philo->data->death_count > 0)
			break ;
		ft_pickup_forks(philo);
		ft_set_state(philo);
	}
	pthread_mutex_unlock(&philo->data->starting_block);
	return (NULL);
}

int	ft_init_threads(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->starting_block);
	while (i < data->philo_c)
	{
		data->philo_array[i].eat_count = 0;
		data->philo_array[i].sleep_count = 0;
		data->philo_array[i].state = 't';
		data->philo_array[i].data = data;
		data->philo_array[i].id = i + 1;
		data->philo_array[i].time_of_last_meal = data->start_time;
		i++;
	}
	i = 0;
	while (i < data->philo_c - 1)
	{
		data->philo_array[i].next_fork = &data->philo_array[i + 1].own_fork;
		i++;
	}
	data->philo_array[i].next_fork = &data->philo_array[0].own_fork;
	i = 0;
	while (i < data->philo_c)
	{
		if (pthread_create(&data->philo_array[i].this_thread, NULL, routine,
				(void *)&data->philo_array[i]) != 0)
		{
			wait_for_philosophers(data, i);
			return(1);
		}
		if (pthread_mutex_init(&data->philo_array[i].own_fork, NULL))
			return (0);
		i++;
	}
	pthread_mutex_unlock(&data->starting_block);
	return (0);
}
int	main(int argc, char **argv)
{
	t_data			data;
	struct timeval	time;

	if (!ft_get_args(argc, argv, &data))
		return (0);
	pthread_mutex_init(&data.print_mutex, NULL);
	data.start_time = get_current_time_in_ms();
	data.philo_array = ft_calloc(data.philo_c, sizeof(t_philosopher));
	data.finished_philosphers = 0;
	gettimeofday(&time, NULL);
	if (ft_init_threads(&data))
	{
		free(data.philo_array);
		return (0);
	}
	pthread_create(&data.monitor, NULL, monitor, &data);
	pthread_join(data.monitor, NULL);
	wait_for_philosophers(&data, data.philo_c);
	return (0);
}
