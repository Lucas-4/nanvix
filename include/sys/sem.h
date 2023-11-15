/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEM_H_
#define SEM_H_

#include <nanvix/const.h>
#include <nanvix/pm.h>

/**
 * @brief Comand values for semaphores.
 */
/**@{*/
#define GETVAL 0   /**< Returns the value of a semaphore. */
#define SETVAL 1   /**< Sets the value of a semaphore.    */
#define IPC_RMID 3 /**< Destroys a semaphore.            */
#define MAX_SEM_NUM 10
#define FIRST_SEM ((&semaphores[0]))
#define LAST_SEM ((&semaphores[MAX_SEM_NUM - 1]))

/**@}*/

/* Forward definitions. */
EXTERN int semget(unsigned);
EXTERN int semctl(int, int, int);
EXTERN int semop(int, int);
struct semaphore
{
    int id;
    unsigned key;

    // number of processes that are using the semaphore
    int count;
    int max_val;
    int curr_val;
    // chain of processes associated with a semaphore id that will be put to sleep when semaphore is 0
    struct process *chain;
};

EXTERN struct semaphore semaphores[MAX_SEM_NUM];

EXTERN void sem_init(void);

#endif /* SEM_H_ */
