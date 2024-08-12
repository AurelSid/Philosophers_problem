/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asideris <asideris@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:41:20 by asideris          #+#    #+#             */
/*   Updated: 2024/08/12 13:44:39 by asideris         ###   ########.fr       */
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
	long	arrival_time;
	long	init_time;

	init_time = get_current_time_in_ms() - philo->data->start_time;
	if (philo->state == 'e')
	{
		print_status(philo->id, philo->data, "is eating");
		ft_usleep(philo->data->t_to_eat);
		pthread_mutex_unlock(philo->next_fork);
		pthread_mutex_unlock(&philo->own_fork);
		philo->state = 's';
		print_status(philo->id, philo->data, "is sleeping");
		ft_usleep(philo->data->t_to_sleep);
		arrival_time = get_current_time_in_ms() - philo->data->start_time;
		if (arrival_time - init_time > philo->data->t_to_die)
		{
			printf("%d IS DEAD\n", philo->id);
			exit(0);
		}
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
		ft_usleep(200);
	while (1)
	{
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
	while (i < data->philo_c )
	{
		data->philo_array[i].eat_count = 0;
		data->philo_array[i].sleep_count = 0;
		data->philo_array[i].state = 't';
		data->philo_array[i].data = data;
		data->philo_array[i].id = i;
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
		pthread_create(&data->philo_array[i].this_thread, NULL, routine,
			(void *)&data->philo_array[i]);
		if (pthread_mutex_init(&data->philo_array[i].own_fork, NULL))
			return (1);
		i++;
	}
	pthread_mutex_unlock(&data->starting_block);
	return (0);
}
int	main(int argc, char **argv)
{
	t_data data;

	struct timeval time;

	ft_get_args(argc, argv, &data);
	pthread_mutex_init(&data.print_mutex, NULL);
	data.start_time = get_current_time_in_ms();
	data.philo_array = ft_calloc(data.philo_c, sizeof(t_philosopher));
	gettimeofday(&time, NULL);
	if (ft_init_threads(&data))
	{
		free(data.philo_array);
		return (0);
	}
	wait_for_philosophers(&data);
	return (0);
}