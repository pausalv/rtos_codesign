---
title: "8. Mailbox. Uso de Interrupciones"
---

## Contexto y objetivo

En ejercicios anteriores usamos semáforos para proteger secciones críticas: el semáforo señaliza que un recurso está disponible, pero no transfiere información. Cuando una ISR o tarea necesita **enviar un dato** a otra tarea, el mecanismo adecuado es el **mailbox** (buzón de mensajes).

Un mailbox almacena un único puntero (`void *`). La tarea productora escribe el puntero (*post*) y la tarea consumidora lo lee (*pend*), bloqueándose si el buzón está vacío. Esto permite desacoplar la generación del dato (la ISR, que ocurre de forma asíncrona) del consumo del dato (la tarea, que lo procesa cuando le toca ejecutarse).

En este ejercicio modificaremos la ISR de pulsadores para que detecte pulsaciones de KEY2 y KEY3 y comunique la nueva posición vertical de Homer a `TaskHomer` a través de un mailbox.

## Servicios µC/OS-II relevantes

**Creación del mailbox:**
```c
OS_EVENT *OSMboxCreate(void *msg);
```
Crea el mailbox con un mensaje inicial. Pase `NULL` si empieza vacío.

**Envío de mensaje (desde ISR o tarea):**
```c
INT8U OSMboxPost(OS_EVENT *pevent, void *msg);
```
Escribe el puntero `msg` en el mailbox. Si ya había un mensaje pendiente de leer, devuelve error.

**Recepción de mensaje (bloquea la tarea):**
```c
void *OSMboxPend(OS_EVENT *pevent, INT32U timeout, INT8U *err);
```
Devuelve el puntero almacenado en el mailbox. Si está vacío, bloquea la tarea hasta que llegue un mensaje o expire el timeout (en ticks). Con timeout 0 espera indefinidamente.

## Implementación

### 1. Creación del mailbox

En `init.c`, declare un puntero `OS_EVENT *SharedMail` y créelo dentro de `UCOS_Utilities()` con `OSMboxCreate(NULL)`. Publíquelo como `extern` en `init.h`.

### 2. Modificación de la ISR de pulsadores

Modifique la función `pushbutton_isr()` en `isr.c` para implementar el siguiente comportamiento:

- **KEY3 pulsado**: decrementa la posición vertical de Homer.
- **KEY2 pulsado**: incrementa la posición vertical de Homer.
- Al finalizar el tratamiento, envía la nueva posición al mailbox con `OSMboxPost`.

Para enviar la posición, necesita un puntero a una variable entera. Declárela como `static` dentro de la ISR para que su dirección sea estable entre invocaciones (las variables locales normales quedan inválidas al salir de la función). Sature el valor de la posición para que no salga de los límites válidos de la pantalla VGA.

> **Recuerde:** la ISR debe llamar a `OSIntEnter()` al principio y a `OSIntExit()` al final para que µC/OS-II pueda gestionar correctamente el retorno de la interrupción.

### 3. Modificación de TaskHomer

Modifique `TaskHomer` para que reciba la posición del mailbox y la use como posición de inicio del dibujo:

- Antes del bucle, declare un puntero local para recibir mensajes del mailbox.
- Dentro del bucle, llame a `OSMboxPend` con un timeout razonable (por ejemplo, 1000 ticks). Si el retorno es `OS_ERR_NONE`, actualice la variable `homer_pos` con el valor recibido.
- Use `homer_pos` al llamar a `VGA_text` para dibujar en la posición correcta.

Compile y pruebe: pulsando KEY3 y KEY2 debe moverse el dibujo de Homer hacia arriba y hacia abajo respectivamente.

### Cuestiones

- ¿Qué diferencias existen entre semáforos y mailbox?
- ¿Qué ventajas puede presentar un mailbox frente a una variable global protegida por semáforo?
- ¿Qué representa el valor de timeout en `OSMboxPend()`?
- ¿Podemos enviar caracteres ASCII por un mailbox? ¿Y una cadena de texto?

Una vez desarrollada la solución muéstrela al profesor y archive el proyecto en un ZIP.

[Ir Ejercicio 9](ex9.md)
[Volver a Indice](index.md)
