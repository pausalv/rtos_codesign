---
title: "5. Sección Crítica: Semáforos e Interrupciones"
---

## Contexto y objetivo

En el ejercicio anterior identificamos que `Print_VGA()` es una función no reentrante porque varias tareas comparten la variable global `line`. Si una tarea es expulsada por el scheduler en medio de una llamada a `Print_VGA()`, otra tarea puede modificar `line` antes de que la primera termine, produciendo salidas corruptas en pantalla.

La solución es proteger el acceso a `Print_VGA()` (y a `printf`) como una **sección crítica**: un fragmento de código que solo puede ejecutarse por una tarea a la vez. En µC/OS-II, la herramienta estándar para implementar secciones críticas entre tareas es el **semáforo binario**.

Un semáforo binario actúa como un token con valor 0 o 1: la tarea que quiere entrar a la sección crítica solicita el token (`Pend`); si está disponible, lo toma y continúa; si no, se bloquea hasta que otra tarea lo devuelva (`Post`). De esta forma se garantiza que como máximo una tarea esté dentro de la sección crítica en cada instante.

## Servicios µC/OS-II relevantes

**Creación del semáforo** (se crea con valor inicial 1 para que la primera llamada a Pend tenga éxito):
```c
OS_EVENT *OSSemCreate(INT16U cnt);
```

**Espera (entrada a sección crítica):**
```c
void OSSemPend(OS_EVENT *pevent, INT32U timeout, INT8U *err);
```
Bloquea la tarea hasta que el semáforo esté disponible. Con `timeout = 0` espera indefinidamente.

**Señal (salida de sección crítica):**
```c
INT8U OSSemPost(OS_EVENT *pevent);
```
Libera el semáforo, desbloqueando la tarea de mayor prioridad que esté esperando.

## Implementación

### Paso 1: creación del semáforo

En `init.c` se dispone de una función `UCOS_Utilities()` destinada a crear los objetos de sincronización del sistema. Descomente las líneas de declaración del semáforo `OS_EVENT *Semaphore` y añada la llamada a `OSSemCreate` dentro de esa función para crearlo con valor inicial 1.

En `init.h` descomente la línea que lo publica como `extern`, para que todas las tareas puedan acceder a él.

`UCOS_Utilities()` se llama desde `Init_App()`, de modo que el semáforo queda creado antes de que arranque ninguna tarea.

<div align="center">
	<img src="img/Imagen19.png" alt="Creación de función UCOS_Utilities()" width="500"/>
	<br>
	<em>Figura 19. Creación de función UCOS_Utilities().</em>
</div>

<div align="center">
	<img src="img/Imagen20.png" alt="Inclusión de funciones de inicio en main()" width="500"/>
	<br>
	<em>Figura 20. Inclusión de funciones de inicio en main().</em>
</div>

### Paso 2: protección de la sección crítica en todas las tareas

Identifique en cada tarea los puntos donde se accede a `Print_VGA()` y a `printf` (que también accede a recursos compartidos). Envuelva esos accesos con `OSSemPend` antes de entrar y `OSSemPost` al salir. Tenga en cuenta:

- No olvide declarar la variable `INT8U err` local en cada tarea para recoger el código de error.
- Cuanto más corta sea la sección crítica (menos código entre Pend y Post), mejor: se reduce el tiempo que las demás tareas están bloqueadas.

Consulte la Figura 21 como referencia del patrón a aplicar.

<div align="center">
	<img src="img/Imagen21.png" alt="Uso de semáforo binario en una task" width="500"/>
	<br>
	<em>Figura 21. Uso de semáforo binario en una task.</em>
</div>

Aplique el mismo patrón a todas las tareas que usen `Print_VGA()` o `printf`. Compile y verifique que la salida por VGA es correcta.

### Preguntas de reflexión

- ¿Por qué se usa un semáforo binario y no uno de valor N?
- ¿Podría utilizarse un MUTEX en lugar de un semáforo binario? ¿Qué ventaja aportaría?
- ¿Qué servicios de µC/OS-II utilizaría con un MUTEX?
- ¿Cuál sería la prioridad de herencia que se generaría en este caso al usar un MUTEX?

## Interrupciones de pulsadores

Ahora vamos a incluir interrupciones de pulsadores en nuestro sistema. Copie los ficheros `isr.c` e `isr.h` entregados en la práctica en su carpeta `AppSW`. Estudie el contenido de ambos.

>[!note] *Descarga ficheros de atención a interrupción:*
>
> - [isr.h](files_ucosii/isr.h) - Archivo de cabecera para rutinas de servicio de interrupción
> - [isr.c](files_ucosii/isr.c) - Archivo fuente con implementación de ISR para pulsadores

Pasos a seguir:

1. En `init.c`, active la habilitación de interrupciones de pulsadores eliminando los comentarios oportunos.
2. Incluya la librería `init.h` en `pract1_rtos.h`.
3. Compile y ejecute sobre NIOSII.

**Pregunta de reflexión:**

- ¿Qué servicios se utilizan para incorporar la ISR al scheduler de µC/OS-II? ¿Por qué son necesarios?

[Ir Ejercicio 6](ex6.md)
[Volver a Indice](index.md)
