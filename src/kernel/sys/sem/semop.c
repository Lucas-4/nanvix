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
#include <nanvix/pm.h>
#include <sys/sem.h>

 /**/
PUBLIC void up(struct semaphore *s)
{

    if (s->curr_val > 0 && s->curr_val < s->max_val)
    {
        s->curr_val++;
    }
    else if (s->curr_val == 0)
    {
        wakeup(&(s->chain));
        s->curr_val++;
    }
}

PUBLIC void down(struct semaphore *s)
{
    if (s->curr_val > 0)
    {
        s->curr_val--;
    }
    else if (s->curr_val == 0)
    {
        sleep(&(s->chain), PRIO_USER + curr_proc->nice);
    }
}

/* Performs up or down operation on a semaphore*/
PUBLIC int sys_semop(int semid, int op)
{
    /* Inavlid parameters */
    if (semid < 0 || op == 0) {
        return -1;
    }

    struct semaphore *sem;
    /* Search the semaphores array for a semaphore with the specified id*/
    for (sem = FIRST_SEM; sem <= LAST_SEM; sem++)
    {
        if (sem->id == semid)
        {
            if (op > 0)
            {
                up(sem);
                return 0;
            }
            else if (op < 0)
            {
                down(sem);
                return 0;
            }
        }
    }

    /* Returns -1 if no semaphore was found */
    return -1;
}