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

PUBLIC int sys_semctl(int semid, int cmd, int val)
{
    struct semaphore *s;

    for (s = FIRST_SEM; s <= LAST_SEM; s++)
    {
        if (s->id == semid)
        {
            if (cmd == GETVAL)
            {
                return s->curr_val;
            }
            if (cmd == SETVAL)
            {
                s->max_val = val;
                s->curr_val = val;
                return 0;
            }
            if (cmd == IPC_RMID)
            {
                s->count--;
                if (s->count == 0)
                {
                    s->id = -1;
                }
            }
        }
    }
}