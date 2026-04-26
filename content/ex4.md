---
title: "4. Uso de VGA y Funciones No Reentrantes"
---

## Contexto y objetivo

Hasta ahora hemos enviado mensajes por UART mediante `printf`. En este ejercicio incorporamos la pantalla VGA como segundo canal de salida. Para ello crearemos una función de inicialización `Init_App()` que configure todos los periféricos antes de que arranque el scheduler.

Sin embargo, este ejercicio tiene un segundo objetivo igual de importante: introducir el concepto de **función no reentrante** y reflexionar sobre los problemas que plantea en un RTOS. Una función es no reentrante cuando su ejecución simultánea desde varios contextos puede producir resultados incorrectos, normalmente porque manipula datos compartidos (variables globales, buffers estáticos). En un RTOS, cualquier tarea puede ser interrumpida en cualquier punto y reemplazada por otra de mayor prioridad, lo que hace que las funciones no reentrantes sean especialmente peligrosas.

## Archivos de inicialización

Añadiremos una función `Init_App()` en un fichero fuente `init.c`, ubicado en `AppSW/src`. Su correspondiente `init.h` irá en `AppSW/inc`.

<!-- >[!note] *Archivos para descargar:*
>
> - [init.h](files_ucosii/init.h) - Archivo de cabecera con prototipos y variables globales
> - [init.c](files_ucosii/init.c) - Archivo fuente con implementación de Init_App() -->

El fichero `init.h` hace públicas las variables globales definidas en `init.c` así como los prototipos de sus funciones. 

El contenido de `init.h` es:

```c
#ifndef INIT_H__
#define INIT_H__

/* include main project library */
#include "../../pract1_rtos.h"

/* declare de functions from init.c */
void Init_App();
void Print_VGA(char visualiza_string[15], int* line);

/* extern public variables */
extern int line;

/* extern UCOS Utilities */
//extern OS_EVENT* Semaphore;

#endif // INIT_H__
```

El contenido de `init.c` es:

```c
#include "..\inc\init.h"


//Text on VGA
char texto_up[80] = 	"                        Understanding RTOS on VGA - CHS 2025/26                ";
char texto_down[80] = 	"                      uCOS-II over NIOS-II, Cyclone V, CHS 2025/26             ";
char blank[80]=         "                                                                               \0";

//Global variable to write lines on VGA
int line=3;

// prototype of UCOS Utilities
void Init_UCOS_Utilities(void);
 
// declaration of UCOS utilities
//OS_EVENT * Semaphore;

// Initialization function
void Init_App()
{
    VGA_Clean_Full_Lines(1,60,character_buffer);

//Text on first line
    VGA_text (2, 0, texto_up,character_buffer);		// first line
    VGA_text (2, 1, texto_down, character_buffer);  // second line

//Boxes on VGA
    VGA_box (0, 0, 80*4-1, 60*4-1, 0x1111,pixel_buffer);	 	//Big blue box on VGA
    VGA_box (0, 0, 320-1, 8, 0x01100,pixel_buffer);  		//Black box on top area of VGA
    /* pink box on right area */
    VGA_box (155, 10, 315, 235, 0xF81F,pixel_buffer);	//Pink box on right area
// VGA_box (155, 10, 315, 235, 0x0100,pixel_buffer);	//Black box on right area


// OFF all leds on board
    Led_OFF_All(LED_ptr);   

// Enable interrupts from pushbuttons
//*(KEY_ptr + 2)=0xF; // Enable interrupts for all 4 pushbuttons
//alt_irq_register(PUSHBUTTONS_IRQ, NULL, pushbutton_isr);

    
/* Initialization of UCOS utilities */
    Init_UCOS_Utilities();

}

/*
 * @brief Prints string on MTL at line position "line"
 * @param input char visualiza_string[40] is the string to be visualized on MTL
 * @param output result of the position to visualize the string
 *
 * @warning non-reentrant function as line is a global shared variable
 */
void Print_VGA(char visualiza_string[35], int* line)
{
    int first_line=3;
    int last_line=60;

    VGA_text (2, *line, visualiza_string, character_buffer);
    VGA_Clean_Lines(*line+1,*line+2,character_buffer);

    *line=*line+1;
    if (*line>last_line){
        *line=first_line;
    }
}

/* UCOS Utilities */
void Init_UCOS_Utilities(void)
{
  //  Semaphore = OSSemCreate(1);
}
```

<!-- <div align="center">
	<img src="img/Imagen16.png" alt="Ejemplo de init.h" width="450"/>
	<br>
	<em>Figura 16. Ejemplo de init.h</em>
</div>

<br> -->

Incluya la llamada a `Init_App()` en `main()` **antes** de la creación de las tareas. Compile y verifique que la VGA funciona.

## Uso de Print_VGA() en las tareas

La función `Print_VGA(char *msg, int *line)` escribe una cadena en la VGA y avanza el puntero de línea `line`. Modifique todas las tareas para que también envíen su mensaje por VGA, usando esta función.

Compile y verifique el funcionamiento.

## Funciones no reentrantes

La función `Print_VGA()` utiliza la variable global `line` como puntero de escritura compartido entre todas las tareas. Si una tarea es expulsada por el scheduler justo mientras está actualizando `line`, y otra tarea empieza a llamar a `Print_VGA()`, el resultado puede ser incorrecto: mensajes solapados, líneas saltadas o escrituras fuera de los límites de la pantalla.

**Preguntas de reflexión:**

- ¿Por qué se considera `Print_VGA()` una función no reentrante?
- ¿Qué solución se debe adoptar cuando en un RTOS trabajamos con funciones no reentrantes?

## Inclusión del número de línea

Modifique las tareas para que el número de línea sea visible en cada mensaje de VGA. Para ello construya una cadena de texto con `snprintf` que incluya el valor de `line` antes del mensaje, y pásela a `Print_VGA()`. Consulte la Figura 18 como referencia.

<div align="center">
	<img src="img/Imagen18.png" alt="Inclusión de número de línea en código de task" width="600"/>
	<br>
	<em>Figura 18. Inclusión de número de línea en código de task.</em>
</div>

<br>

**Importante:** Archive su proyecto en ZIP para no perder el trabajo hasta este punto.

[Ir Ejercicio 5](ex5.md)
[Volver a Indice](index.md)
