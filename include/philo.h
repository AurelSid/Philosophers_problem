#ifndef PHILO_H
# define PHILO_H

# include "../libft/libft.h"
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philosopher
{
	char				state;
	int					eat_count;
	int					sleep_count;
	int					id;
	pthread_t			this_thread;
	pthread_mutex_t		own_fork;
	pthread_mutex_t		*next_fork;
	t_data				*data;

}						t_philosopher;

typedef struct s_data
{
	int					philo_c;
	int					t_to_die;
	int					t_to_eat;
	int					min_meals;
	int					t_to_sleep;
	t_philosopher		*philo_array;
	pthread_mutex_t		starting_block;
	pthread_mutex_t		routine_lock;
	pthread_mutex_t		print_lock;
	long				start_time;
	pthread_mutex_t		print_mutex;
	int					death_count;
	pthread_t			monitor;

}						t_data;

int						ft_get_args(int argc, char **argv, t_data *data);
int						ft_print_philos(t_data *data);
int						ft_pickup_forks(t_philosopher *philo);
void					wait_for_philosophers(t_data *data);
int						ft_usleep(size_t milliseconds);
long					get_current_time_in_ms(void);
void					print_status(int philosopher_id, t_data *data,
							const char *status);

#endif
