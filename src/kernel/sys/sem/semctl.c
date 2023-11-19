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

 /* Performs control operation on a semaphore */
PUBLIC int sys_semctl(int semid, int cmd, int val)
{
    /* Inavlid parameters */
    if (semid < 0 || !(cmd == GETVAL || cmd == SETVAL || cmd == IPC_RMID)) {
        return -1;
    }


    struct semaphore *s;

    for (s = FIRST_SEM; s <= LAST_SEM; s++)
    {
        if (s->id == semid)
        {
            /* Returns the current value of the semaphore */
            if (cmd == GETVAL)
            {
                return s->curr_val;
            }
            /* Sets the value of the semaphore */
            if (cmd == SETVAL)
            {
                s->max_val = val;
                s->curr_val = val;
                return 0;
            }
            /* Destroy a semaphore */
            if (cmd == IPC_RMID)
            {
                s->id = -1;
                return 0;
            }
        }
    }

    /* Returns -1 if no semaphore was found */
    return -1;
}