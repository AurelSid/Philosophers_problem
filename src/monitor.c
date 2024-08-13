/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asideris <asideris@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 13:51:13 by asideris          #+#    #+#             */
/*   Updated: 2024/08/13 16:32:34 by asideris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_found_dead(t_data *data)
{
	long	current_time;

	pthread_mutex_lock(&data->death_count_mutex);
	current_time = get_current_time_in_ms() - data->start_time;
	printf("\033[0;31m%-15ld %-5d IS DEAD <------------ !!\033[0m\n",
		get_current_time_in_ms() - data->start_time, data->death_count);
	pthread_mutex_unlock(&data->death_count_mutex);
	wait_for_philosophers(data);
	exit(0);
	usleep(100);
}

void	*monitor(void *arg)
{
	t_data	*data;
	long	elapsed_time;
	long	current_time;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->philo_c)
		{
			if (data->finished_philosphers >= data->philo_c)
			{
				printf("FINISHED------------************************\n");
				wait_for_philosophers(data);
				exit(0);
				usleep(100);
			}
			current_time = get_current_time_in_ms() - data->start_time;
			elapsed_time = current_time
				- data->philo_array[i].time_since_last_meal;
			if (elapsed_time > data->t_to_die)
			{
				data->death_count = i + 1;
				ft_found_dead(data);
			}
		}
	}
	return (NULL);
}

int	ft_reset_clock(t_philosopher *philo)
{
	long	current_time;

	current_time = get_current_time_in_ms() - philo->data->start_time;
	if (philo->time_since_last_meal == 0)
	{
		philo->time_since_last_meal = philo->data->start_time;
		philo->time_of_new_meal = current_time;
		if ((current_time
				- philo->time_since_last_meal) > philo->data->t_to_die)
		{
			pthread_mutex_lock(&philo->data->death_count_mutex);
			philo->data->death_count = philo->id;
			pthread_mutex_unlock(&philo->data->death_count_mutex);
			return (0);
		}
		return (1);
	}
	if ((current_time - philo->time_since_last_meal) > philo->data->t_to_die)
	{
		pthread_mutex_lock(&philo->data->death_count_mutex);
		philo->data->death_count = philo->id;
		pthread_mutex_unlock(&philo->data->death_count_mutex);
		return (0);
	}
	philo->time_since_last_meal = current_time;
	return (1);
}
