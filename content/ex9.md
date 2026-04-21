---
title: "9. Colas de Mensajes. Uso de PS2"
---

## Contexto y objetivo

El mailbox del ejercicio anterior almacena un único mensaje: si se envía uno nuevo antes de que el consumidor lea el anterior, el mensaje se pierde. Cuando el productor puede generar varios mensajes en ráfaga (como un teclado), necesitamos una **cola de mensajes** (*message queue*): una estructura FIFO capaz de almacenar múltiples punteros a mensajes.

En este ejercicio implementaremos un sistema de autenticación por teclado PS2: el usuario teclea un código de tres dígitos seguido de INTRO, y el sistema indica si es correcto o incorrecto.

## El teclado PS2

El teclado PS2 envía códigos de *make* (tecla pulsada) y *break* (tecla soltada). El código de *break* siempre es `0xF0` seguido del código de la tecla. Por tanto, para detectar que una tecla ha sido soltada, la ISR debe esperar el byte `0xF0` y luego leer el siguiente byte como el código definitivo.

El teclado envía el código `0xAA` al conectarse (cuando recibe alimentación). El código de Intro es `0x5A`.

Para verificar si el dato del teclado es válido, compruebe el bit RVALID del registro DATA del periférico PS2 antes de leer el código.

## Servicios µC/OS-II relevantes

**Creación de la cola** (requiere un array de punteros y su tamaño):
```c
OS_EVENT *OSQCreate(void **start, INT16U size);
```

**Envío de mensaje a la cola:**
```c
INT8U OSQPost(OS_EVENT *pevent, void *msg);
```
Añade el puntero `msg` al final de la cola. Devuelve error si la cola está llena.

**Recepción de mensaje de la cola:**
```c
void *OSQPend(OS_EVENT *pevent, INT32U timeout, INT8U *err);
```
Extrae el primer mensaje de la cola. Bloquea la tarea si está vacía.

**Vaciado de la cola:**
```c
INT8U OSQFlush(OS_EVENT *pevent);
```
Elimina todos los mensajes pendientes de la cola.

## Diseño de la solución

Antes de programar, piense en el diseño. El sistema tiene dos actores:

- **ISR del teclado PS2** (productora): se activa con cada tecla pulsada y envía el código a la cola.
- **TaskCode** (consumidora): lee mensajes de la cola periódicamente y comprueba si el código introducido es correcto.

Considere los siguientes casos:
- El usuario puede teclear a cualquier velocidad.
- Puede no pulsar Intro, lo que debe considerarse código incorrecto.
- Puede teclear más de tres dígitos antes del Intro; tampoco es correcto.
- El código correcto son exactamente 3 teclas + Intro (4 pulsaciones en total).

> Dedique unos minutos a pensar el diseño antes de programar. Discuta con el profesor si está en el laboratorio.

## Estructura de datos del mensaje

Para almacenar en cada mensaje no solo el código de tecla sino también información temporal y de orden, defina la siguiente estructura en `init.h`:

```c
#define QUEUE_SIZE 4

typedef struct msg_data {
    int time;
    int key_pressed;
    int count_key;
} MSG_DATA_PS2;
```

## Implementación

### En init.c / init.h

Declare e inicialice en `UCOS_Utilities()`:
- El puntero al evento de cola: `OS_EVENT *MessageQueue`
- El array de punteros que actúa como buffer: `void *MessageQueueTbl[QUEUE_SIZE]`
- Cree la cola con `OSQCreate`

Active la interrupción del teclado PS2 en `Init_App()` registrando la ISR correspondiente:
```c
alt_irq_register(PS2_KEY_IRQ, NULL, (alt_isr_func) isr_ps2_keyboard);
```

### ISR del teclado (productora)

Cree la función `keyboard_isr()` en `isr.c`. Esta función debe:

1. Llamar a `OSIntEnter()` al inicio y `OSIntExit()` al final.
2. Deshabilitar la interrupción PS2 al entrar (`alt_irq_disable`) y rehabilitarla al salir (`alt_irq_enable`).
3. Comprobar si el dato es válido (bit RVALID).
4. Gestionar la secuencia make/break: usar una variable `static` para recordar si el byte anterior fue `0xF0`.
5. Cuando se reciba el código de una tecla soltada, rellenar una estructura `MSG_DATA_PS2` (con el tiempo actual `OSTimeGet()`, el código y un contador) y enviarla a la cola con `OSQPost`.

> Declare el array de estructuras `MSG_DATA_PS2 msg_isr[QUEUE_SIZE]` como `static` dentro de la ISR. Así la memoria se reserva en tiempo de compilación y los punteros enviados a la cola siguen siendo válidos tras salir de la ISR.

### TaskCode (consumidora)

Cree la tarea `TaskCode` en `taskcode.c`. Esta tarea debe ejecutarse periódicamente (por ejemplo cada segundo) y:

1. Leer un mensaje de la cola con `OSQPend`.
2. Acumular los códigos recibidos en una variable de 32 bits (desplazando 8 bits cada vez).
3. Llevar la cuenta de teclas recibidas.
4. Si se recibe Intro (`0x5A`): comparar el código acumulado contra el código secreto, mostrar CORRECTO o INCORRECTO en VGA, vaciar la cola y resetear contadores.
5. Si aún no se han recibido 4 teclas, mostrar un mensaje de "introduciendo código".
6. Si se han recibido más de 4 teclas sin Intro, tratar como código incorrecto.

El código secreto puede ser la secuencia 1-2-3-Intro. Tenga en cuenta que los códigos de las teclas numéricas del teclado numérico difieren de los del teclado principal.

Compile, ejecute y pruebe el sistema.

### Cuestiones

- ¿Por qué se usa una cola y no un mailbox para este ejercicio?
- ¿Por qué se declara el array de mensajes de la ISR como `static`?
- ¿Qué ocurre si `OSQPost` devuelve error porque la cola está llena?
- ¿Qué representa el segundo parámetro de `OSQPend`?

[Ir Ejercicio 10](ex10.md)
[Volver a Indice](index.md)
