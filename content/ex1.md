---
title: "1. Creación de Tareas"
---

Modifique el nombre del fichero principal `hello_ucosii.c` por `pract1_rtos.c`.

Estudie el fichero ejemplo proporcionado y cree dos tareas adicionales, una con periodo de 1 seg y otra con periodo de 5 seg, con la misma funcionalidad que las tareas 1 y 2 ejemplo. Cada tarea debe mandar un mensaje `"Hello from task i"` con `i` = número de tarea. Las nuevas tareas tendrán prioridades 3 y 4 y sus pilas de tareas con el mismo tamaño que las tareas previas.

Compile y observe los resultados obtenidos. Recuerde capturar pantalla y redactar el documento de entrega de la práctica.

### Plantilla de código para completar

El siguiente fragmento está pensado para que el estudiante lo pueda copiar completo y rellenar las zonas marcadas con `TODO 1`, `TODO 2`, etc.:

```c
#include <stdio.h>
#include "includes.h"

/* Tamaño de pila de cada tarea */
#define TASK_STACKSIZE  2048

/* Pilas de tareas */
OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];
/* TODO 1: pila de la tarea 3 */
/* TODO 2: pila de la tarea 4 */

/* Prioridades de las tareas */
#define TASK1_PRIORITY  1
#define TASK2_PRIORITY  2
/* TODO 3: prioridad de la tarea 3 */
/* TODO 4: prioridad de la tarea 4 */

/* Prototipos */
void task1(void* pdata);
void task2(void* pdata);
/* TODO 5: prototipo de la tarea 3 */
/* TODO 6: prototipo de la tarea 4 */

/* Tarea 1: periodo ~3 seg */
void task1(void* pdata)
{
	while (1)
	{
		printf("Hello from task1\n");
		OSTimeDlyHMSM(0, 0, 3, 0);
	}
}

/* Tarea 2: periodo ~3 seg */
void task2(void* pdata)
{
	while (1)
	{
		printf("Hello from task2\n");
		OSTimeDlyHMSM(0, 0, 3, 0);
	}
}

/* Tarea 3 (nueva): periodo 1 seg */
void task3(void* pdata)
{
	while (1)
	{
		/* TODO 7: escriba el mensaje de la tarea 3 */
		/* TODO 8: añada un retardo de 1 segundo */
	}
}

/* Tarea 4 (nueva): periodo 5 seg */
void task4(void* pdata)
{
	while (1)
	{
		/* TODO 9: escriba el mensaje de la tarea 4 */
		/* TODO 10: añada un retardo de 5 segundos */
	}
}

int main(void)
{
	OSTaskCreateExt(task1, NULL, &task1_stk[TASK_STACKSIZE-1],
					TASK1_PRIORITY, TASK1_PRIORITY, task1_stk,
					TASK_STACKSIZE, NULL, 0);

	OSTaskCreateExt(task2, NULL, &task2_stk[TASK_STACKSIZE-1],
					TASK2_PRIORITY, TASK2_PRIORITY, task2_stk,
					TASK_STACKSIZE, NULL, 0);

	/* TODO 11: creación de la tarea 3 */

	/* TODO 12: creación de la tarea 4 */

	OSStart();  /* Inicia el scheduler de uCOS-II */

	return 0;
}
```
> **Nota:** `OSTimeDlyHMSM(h, m, s, ms)` suspende la tarea el tiempo indicado en horas, minutos, segundos y milisegundos. `OSStart()` arranca el scheduler y no retorna nunca.

### Cuestiones

- ¿Qué servicios son necesarios para crear una tarea?
- ¿Qué elementos necesita una tarea para ser creada?
- ¿Cuándo se inicia el scheduler de tareas de UCOS y las tareas empiezan a funcionar?

[Ir Ejercicio 2](ex2.md)
[Volver a Índice](index.md)
