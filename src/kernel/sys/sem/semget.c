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

 /* Creates a semaphore */
PUBLIC int sys_semget(unsigned key)

{
	struct semaphore *sem;

	// search the semaphore array for a semaphore with the specified key and returns the id
	for (sem = FIRST_SEM; sem <= LAST_SEM; sem++)
	{
		if (sem->key == key && sem->id >= 0)
		{
			return sem->id;
		}
	}

	/*
	If no semaphore with the specified key exists, search the semaphores array again for a semaphore with a negative ID.
	(A negative ID indicates that the semaphore is destroyed)
	*/
	int i = 0;
	for (sem = FIRST_SEM; sem <= LAST_SEM; sem++)
	{
		if (sem->id < 0)
		{
			sem->key = key;
			// The semaphore ID is always set to the index of the semaphore in the array
			sem->id = i;

			return sem->id;
		}
		i++;
	}

	/* Returns -1 if can't create a new semaphore */
	return -1;
}