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

#include <nanvix/const.h>
#include <sys/sem.h>

/*Array of semaphores*/
PUBLIC struct semaphore semaphores[MAX_SEM_NUM];

/*Function used to initialize the semaphores*/
PUBLIC void sem_init(void)
{
    struct semaphore *sem;

    for (sem = FIRST_SEM; sem <= LAST_SEM; sem++)
    {
        sem->id = -1;
        sem->key = 0;
        sem->chain = NULL;
    }
}