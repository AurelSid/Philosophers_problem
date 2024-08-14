/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asideris <asideris@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:41:20 by asideris          #+#    #+#             */
/*   Updated: 2024/08/14 15:25:34 by asideris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_print_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_c)
	{
		printf(" Philo n: %d", i);
		printf(" | meals: %d", data->philo_array[i].eat_count);
		printf(" | sleeps: %d", data->philo_array[i].sleep_count);
		printf(" | status: %c", data->philo_array[i].state);
		printf(" | own_fork: %p", &data->philo_array[i].own_fork);
		printf(" | next_fork: %p\n", &*data->philo_array[i].next_fork);
		i++;
	}
	return (0);
}

int	ft_pickup_forks(t_philosopher *philo)
{
	if (pthread_mutex_lock(&philo->own_fork) == 0)
	{
		if (philo->data->finished_philosphers < philo->data->philo_c
			&& philo->data->death_count == 0)
		{
			pthread_mutex_lock(&philo->data->print_mutex);
			printf("%-15ld %-5d %-15s\n", get_current_time_in_ms()
				- philo->data->start_time, philo->id, "picked up fork");
			pthread_mutex_unlock(&philo->data->print_mutex);
		}
	}
	if (pthread_mutex_lock(philo->next_fork) == 0)
	{
		if (philo->data->finished_philosphers < philo->data->philo_c
			&& philo->data->death_count == 0)
		{
			pthread_mutex_lock(&philo->data->print_mutex);
			printf("%-15ld %-5d %-15s\n", get_current_time_in_ms()
				- philo->data->start_time, philo->id, "picked up fork");
			pthread_mutex_unlock(&philo->data->print_mutex);
		}
		philo->state = 'e';
		ft_reset_clock(philo);
	}
	return (0);
}

void	wait_for_philosophers(t_data *data, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		pthread_join(data->philo_array[i].this_thread, NULL);
		i++;
	}
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time_in_ms();
	while ((get_current_time_in_ms() - start) < milliseconds)
		usleep(500);
	return (0);
}

void	print_status(int philosopher_id, t_data *data, const char *status)
{
	long	timestamp;

	timestamp = get_current_time_in_ms() - data->start_time;
	if (data->finished_philosphers < data->philo_c && data->death_count == 0)
	{
		pthread_mutex_lock(&data->print_mutex);
		printf("%-15ld %-5d %-15s\n", timestamp, philosopher_id, status);
		pthread_mutex_unlock(&data->print_mutex);
	}
}
