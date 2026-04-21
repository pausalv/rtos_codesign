---
title: "7. Suspensión de Tareas y Reactivación"
---

## Contexto y objetivo

En µC/OS-II, una tarea puede encontrarse en varios estados: **lista** (*ready*), **ejecutando** (*running*), **esperando** un evento o retardo (*waiting*) o **suspendida** (*suspended*). Los estados waiting y suspended se diferencian en que waiting tiene una condición de salida automática (un tiempo o un evento), mientras que suspended solo puede salirse desde otra tarea o desde la propia tarea si se recupera a sí misma.

Esta capacidad es útil para pausar el comportamiento de una tarea ante ciertas condiciones del sistema sin eliminarla completamente, y recuperarla cuando sea necesario. En este ejercicio crearemos una tarea que controla el estado de otras tareas del sistema leyendo los switches de la placa.

## Servicios µC/OS-II relevantes

**Suspensión de una tarea:**
```c
INT8U OSTaskSuspend(INT8U prio);
```
Suspende la tarea con la prioridad indicada. Retorna `OS_ERR_NONE` si tiene éxito. Una tarea no puede suspenderse a sí misma con este servicio de forma segura si es la única que puede recuperarla.

**Recuperación de una tarea suspendida:**
```c
INT8U OSTaskResume(INT8U prio);
```
Reactiva una tarea que estaba en estado suspended. No tiene efecto sobre tareas en otros estados.

**Borrado de una tarea:**
```c
INT8U OSTaskDel(INT8U prio);
```
Elimina definitivamente la tarea con la prioridad indicada del scheduler. No puede deshacerse.

## Implementación de TaskSW

Cree los ficheros `tasksw.c` y `tasksw.h` en las carpetas correspondientes de `AppSW`.

La tarea debe ejecutarse cada 1,5 segundos y leer el valor de los switches de la placa. La interpretación de los switches es la siguiente:

| SW1 | SW0 | Acción                                        |
|-----|-----|-----------------------------------------------|
|  0  |  0  | Sin acción                                    |
|  0  |  1  | Suspender la tarea cuya prioridad indica SW5:SW2 |
|  1  |  0  | Recuperar la tarea cuya prioridad indica SW5:SW2 |
|  1  |  1  | Borrar la tarea cuya prioridad indica SW5:SW2    |

Los bits SW5:SW2 (4 bits) indican la prioridad de la tarea sobre la que actuar.

**Consideración importante:** una tarea no debe suspenderse a sí misma, ya que quedaría bloqueada sin posibilidad de recuperar las demás. Asegúrese de que `TaskSW` no actúa sobre su propia prioridad.

Implemente la lógica usando `switch/case` sobre la acción leída. Recuerde:
- Usar el semáforo para proteger los accesos a `printf`.
- Usar `alt_ucosii_check_return_code(err)` para detectar errores en los servicios.
- Imprimir por consola y VGA la acción realizada y la prioridad afectada.

Registre la tarea en `main()` con una prioridad adecuada y añada su cabecera a `pract1_rtos.h`.

Compile, ejecute y pruebe suspender, recuperar y borrar las tareas de Bart y Homer desde los switches.

### Cuestiones

- ¿Cómo funcionan los servicios de suspensión y recuperación? Comente el funcionamiento del diagrama de estados de una tarea en µC/OS-II.
- ¿Pueden suspenderse tareas de prioridad superior a la tarea desde la que se lanza la suspensión?
- ¿Qué diferencia hay entre suspender y borrar una tarea?

[Ir Ejercicio 8](ex8.md)
[Volver a Indice](index.md)
