# Semáforos no Nanvix

## Introdução

Neste trabalho foi implementado o semáforo, que é uma das mais conhecidas primitivas de sincronização entre processos.

## Implementação

### Definição do semáforo

O semáforo é definido em uma struct no arquivo include/sys/sem.h

```c

#define GETVAL 0   /**< Returns the value of a semaphore. */
#define SETVAL 1   /**< Sets the value of a semaphore.    */
#define IPC_RMID 3 /**< Destroys a semaphore.            */
#define MAX_SEM_NUM 10
#define FIRST_SEM ((&semaphores[0]))
#define LAST_SEM ((&semaphores[MAX_SEM_NUM - 1]))

struct semaphore
{
    int id;
    unsigned key;
    int max_val;
    int curr_val;
    // chain where the processes will be put to sleep when the semaphore is 0
    struct process *chain;
};
```

### Tabela e inicialização

No arquivo src/kernel/sys/sem.c está a definição da tabela/vetor de semáforos e a função de inicialização da tabela de semáforos (essa função é chamada no arquivo src/kernel/init/main.c)

```c
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
```

### Criando um semáforo

No arquivo src/kernel/sys/sem/semget.c está a função de criação do semáforo

Esta função recebe um número inteiro positivo que é usado como uma chave para identificar o semáforo

Ela retorna o id do semáforo ou -1 em caso de error

```c
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
```

### Controlando um semáforo 

No arquivo src/kernel/sys/sem/semctl.c está a função de controle do semáforo

A função recebe o id do semáforo, o número do comando e o valor que o semáforo irá receber

Ela retorna 0 em caso de sucesso na operação ou -1 em caso de erro


Existem 3 comandos possíveis:

GETVAL(0) que retorna o valor atual do semáforo

SETVAL(1) que define o valor atual do semáforo como o valor contido no parametro val

IPC_RMID(3) que destrói o semáforo

```c
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
```

### Operações no semáforo

No arquivo src/kernel/sys/sem/semop.c está a função que performa as operações de up e down no semáforo

A função recebe o id do semáforo e um número que indica qual operação realizar no semáforo, se esse número for maior que 0 realiza a operação UP e se for menor que 0 realiza a operação DOWN

Ela retorna 0 em caso de sucesso ou -1 em caso de erro
```c
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
```