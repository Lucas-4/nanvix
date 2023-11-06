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

PUBLIC void up(struct semaphore *s)
{
    if (s->curr_val == 0)
    {
        wakeup(s->chain);
    }
    s->curr_val++;
}

PUBLIC void down(struct semaphore *s)
{
    if (s->curr_val == 0)
    {
        sleep(s->chain);
    }
    else
    {
        s->curr_val--;
    }
}

PUBLIC int sys_semop(int semid, int op)
{
    struct semaphore *sem;
    for (sem = FIRST_SEM; sem <= LAST_SEM; sem++)
    {
        if (sem->id == semid)
        {
            if (op > 0)
            {
                up(sem);
            }
            else if (op < 0)
            {
                down(sem);
            }
        }
    }
}