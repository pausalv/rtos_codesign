---
title: "1. Creación de Tareas"
---

## Contexto y objetivo

En un sistema operativo en tiempo real como µC/OS-II, la unidad básica de ejecución es la **tarea** (*task*). A diferencia de un programa secuencial, múltiples tareas conviven en el mismo procesador y el *scheduler* decide cuál se ejecuta en cada instante según sus prioridades: a menor número, mayor prioridad.

Cada tarea es una función C que contiene un bucle infinito (nunca retorna) y dispone de su propia **pila de ejecución** independiente. Esta pila almacena el contexto de la tarea (registros del procesador, variables locales) cuando el scheduler la desaloja para dar paso a otra de mayor prioridad. El scheduler no comienza a funcionar hasta que se invoca `OSStart()`: antes de ese punto solo se ejecuta el `main()`, donde se crean y registran todas las tareas del sistema.

El objetivo de este ejercicio es crear tareas en µC/OS-II: definir sus pilas, asignar prioridades y registrarlas antes de arrancar el scheduler.

## Servicios µC/OS-II relevantes

**Creación de tarea:**
```c
OSTaskCreateExt(void (*task)(void*), void *p_arg, OS_STK *ptos,
                INT8U prio, INT16U id, OS_STK *pbos,
                INT32U stk_size, void *pext, INT16U opt);
```
Parámetros principales: `task` (función de la tarea), `p_arg` (argumento, puede ser `NULL`), `ptos` (`&stk[SIZE-1]`, tope de pila en arquitecturas descendentes), `prio` (prioridad única en el sistema), `pbos` (base de pila), `stk_size` (tamaño en palabras).

**Retardo temporal:**
```c
OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U milli);
```
Suspende la tarea el tiempo indicado cediendo el procesador a la siguiente tarea lista.

**Arranque del scheduler:**
```c
OSStart(void);  /* No retorna nunca */
```

## Tarea a realizar

Modifique el nombre del fichero principal `hello_ucosii.c` por `pract1_rtos.c`.

Estudie el fichero ejemplo proporcionado y cree dos tareas adicionales: una con periodo de 1 s y otra con periodo de 5 s, con la misma funcionalidad que las tareas 1 y 2 ejemplo. Cada tarea debe mandar un mensaje `"Hello from task i"` con `i` = número de tarea. Las nuevas tareas tendrán prioridades 3 y 4 y sus pilas de tareas con el mismo tamaño que las tareas previas.

Compile y observe los resultados obtenidos. Recuerde capturar pantalla y redactar el documento de entrega de la práctica.

### Plantilla de código para completar

El siguiente fragmento está pensado para que el estudiante lo pueda copiar completo y rellenar las zonas marcadas con `TODO`:

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

/* Tarea 1: periodo ~500 ms */
void task1(void* pdata)
{
    while (1)
    {
        printf("Hello from task1\n");
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

/* Tarea 2: periodo ~500 ms */
void task2(void* pdata)
{
    while (1)
    {
        printf("Hello from task2\n");
        OSTimeDlyHMSM(0, 0, 0, 500);
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

    /* TODO 11: creación de la tarea 3 (siga el patrón de las tareas anteriores) */

    /* TODO 12: creación de la tarea 4 */

    OSStart();  /* Inicia el scheduler de uCOS-II */

    return 0;
}
```

### Cuestiones

- ¿Qué servicios son necesarios para crear una tarea?
- ¿Qué elementos necesita una tarea para ser creada?
- ¿Cuándo se inicia el scheduler de tareas de µC/OS-II y las tareas empiezan a funcionar?

[Ir Ejercicio 2](ex2.md)
[Volver a Índice](index.md)
