---
title: "3. Definición de Periféricos y LEDs"
---

## Contexto y objetivo

En un sistema embebido como la DE1-SoC con procesador NIOS II, los periféricos (LEDs, pulsadores, pantalla VGA, etc.) se controlan mediante **acceso directo a memoria** (*memory-mapped I/O*): cada periférico tiene asignada una dirección de memoria a través de la cual se leen o escriben sus registros de control y datos. Para usar un periférico desde C basta con definir un puntero a esa dirección y leer o escribir a través de él.

En este ejercicio incorporamos el acceso a los periféricos de la placa y usamos las funciones del driver de LEDs para visualizar la actividad de las tareas. El objetivo es entender cómo declarar globalmente los punteros a periféricos y usarlos desde distintas tareas a través de la jerarquía de cabeceras del proyecto.

## Definición de punteros de periféricos

En el fichero principal `pract1_rtos.c` se definen los punteros a las direcciones de memoria de los periféricos. Las constantes con la dirección base de cada periférico (p. ej. `LEDS_BASE`) están definidas en el fichero `system.h` generado por el BSP.

```c
INT8U  error;

/* definition of peripheral addresses */
volatile int *LED_ptr = (int *) LEDS_BASE;
volatile int *SW_swith_ptr  = (int *) SWITCHES_BASE;
volatile int *KEY_ptr = (int *) PUSHBUTTONS_BASE;
volatile int *HEX2_HEX0_ptr = (int *) HEX2_HEX0_BASE;
volatile int *HEX5_HEX3_ptr = (int *) HEX5_HEX3_BASE;

/* definition of VGA pixel buffer */
#define VIDEO_SDRAM_BASE  0x03E00000
#define SDRAM_BASE_SIN_CACHE (SDRAM_BASE + VIDEO_SDRAM_BASE + NIOS2_DCACHE_BYPASS_MASK)

volatile short *pixel_buffer = (short *) SDRAM_BASE_SIN_CACHE;
volatile char *character_buffer = (char *) VIDEO_CHARACTER_BUFFER_WITH_DMA_AVALON_CHAR_BUFFER_SLAVE_BASE;
volatile int *keyboard_ptr = (int *) PS2_KEY_BASE;
```

Para que las tareas en otros ficheros puedan usar estos punteros, deben declararse como `extern` en el fichero de cabecera principal `pract1_rtos.h`. También podemos mover la declaración de las prioridades de las tareas a ese fichero. Así, cualquier tarea que incluya dicha cabecera tiene acceso a todos los periféricos y prioridades.

<!-- <div align="center">
	<img src="img/Imagen13.png" alt="Fichero de cabecera pract1_rtos.h" width="300"/>
	<br>
	<em>Figura 13. Fichero de cabecera pract1_rtos.h.</em>
</div>
-->


```c
/* Definition of Task Priorities */
#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3
#define TASK4_PRIORITY      4

/* public declaration of peripherals */
extern volatile int    * LED_ptr;
extern volatile int    * HEX2_HEX0_ptr;
extern volatile int    * HEX5_HEX3_ptr;
extern volatile int    * KEY_ptr;
extern volatile int    * SW_switch_ptr;
extern volatile short  * pixel_buffer;
extern volatile char   * character_buffer;
extern volatile int    * keyboard_ptr;



```

Se deben incluir todas las librerías en `pract1_rtos.h` y que este sea el único fichero a incluir en cada `.h` de las tareas.
El fichero `task1.h` simplemente declara el prototipo e incluye la cabecera principal del proyecto:

<!-- <div align="center">
	<img src="img/Imagen14.png" alt="Ejemplo de fichero de cabecera de task1.h" width="300"/>
	<br>
	<em>Figura 14. Ejemplo de fichero de cabecera de la task1 (task1.h).</em>
</div>
-->


```c
#ifndef TASK1_H
#define TASK1_H

/* include main project library */
#include "../../pract1_rtos.h"

/* declare functions from task1.c */
void task1(void *pdata);

#endif /* TASK1_H */
```

Compile para verificar que las inclusiones no dan problemas.

## Uso del driver de LEDs

Estudie los ficheros `led.c` y `led.h` para conocer las funciones disponibles. Con ellas implemente el siguiente comportamiento:

- `task1`: usa `Toggle_Led(LED_ptr, 0)` antes y después del `printf`, de forma que el LED 0 parpadee durante su ejecución.
- `task2`: igual que task1 pero con el LED 1.
- `task3` (periodo 1 s): enciende varios LEDs al ejecutarse con `Led_ON_Some(LED_ptr, 8, 2)`.
- `task4` (periodo 5 s): apaga los LEDs que enciende task3 con `Led_OFF_Some(LED_ptr, 8, 2)`.

Para que el resultado sea determinista, vamos a inicializar el estado de los LEDs 0 y 1 en las tareas 1 y 2 respectivamente, de forma que siempre empiecen apagados. Para ello, añada `Led_OFF_Some(LED_ptr, 1, 0)` al inicio de `task1` y `Led_OFF_Some(LED_ptr, 1, 1)` al inicio de `task2`.

A modo de referencia, el cuerpo de `task2.c` quedaría así:

```c
#include "..\inc\task2.h"

void task2(void* pdata)
{
  Led_OFF_Some(LED_ptr, 1, 1); /* inicializa el LED 1 apagado */
  while (1)
  { 
    Toggle_Led(LED_ptr, 1);
    printf("Hello from task2\n");
    Toggle_Led(LED_ptr, 1);
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}
```

Aplique el mismo patrón al resto de tareas y compile el proyecto. Capture los resultados.

### Cuestiones

- ¿Qué indican los LEDs parpadeantes de las tareas 1 y 2?
- ¿A qué se debe la escasa duración de su iluminación?

[Ir Ejercicio 4](ex4.md)
[Volver a Indice](index.md)
