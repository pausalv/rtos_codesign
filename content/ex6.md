---
title: "6. Tareas Bart y Homer"
---

## Contexto y objetivo

En este ejercicio crearemos dos tareas periódicas que dibujan figuras ASCII en la pantalla VGA. El objetivo no es solo aprender a usar las funciones de dibujo, sino también observar la **convivencia temporal** de múltiples tareas con distintos periodos y reflexionar sobre qué ocurre cuando varias tareas intentan dibujar simultáneamente.

Cada tarea tiene su propio periodo de ejecución. Al cambiar la posición del dibujo o limpiar la pantalla, es importante hacerlo de forma que no interfiera con lo que otras tareas están escribiendo. Preste atención a si necesita proteger los accesos a VGA con el semáforo del ejercicio anterior.

## Funciones VGA disponibles

Estudie los ficheros `vga.c` y `vga.h` para conocer las funciones disponibles. Las más relevantes para este ejercicio son:

```c
/* Escribe una cadena en la posición (x, y) del buffer de texto VGA */
void VGA_text(int x, int y, char *text, volatile char *character_buffer);

/* Limpia un rango de líneas completas en el buffer de texto */
void VGA_Clean_Full_Lines(int y_start, int y_end, volatile char *character_buffer);
```

## Tarea TaskBart

Cree los ficheros `taskbart.c` y `taskbart.h` en `AppSW/src` y `AppSW/inc` respectivamente.

La tarea debe dibujar la siguiente figura ASCII en la pantalla VGA en una posición vertical configurable (`bart_pos`), con un periodo de 4 segundos. Antes de dibujar, debe limpiar el área de texto para borrar el dibujo anterior.

Los datos del dibujo son:

```c
char visualiza_string_bart[15][40] = {
    "                 ",
    "    |\\/\\/\\/|     ",
    "    |      |     ",
    "    |      |     ",
    "    | (o)(o)     ",
    "    C      _)    ",
    "    | ,___|      ",
    "    |   /        ",
    "   /____\\        ",
    "  /      \\       ",
    "                 ",
    " EAT MY SHORTS!  ",
    "                 ",
    "                 ",
    "                 "
};
```

Con estos datos y las funciones VGA disponibles, implemente la tarea completa: bucle infinito, limpieza del área, dibujo línea a línea usando `VGA_text`, retardo apropiado y encendido de LEDs como indicador de actividad.

>[!note] *Archivos de referencia (úselos solo si se atasca):*
>
> - [taskbart.h](files_ucosii/taskbart.h) - Archivo de cabecera para la tarea Bart
> - [taskbart.c](files_ucosii/taskbart.c) - Archivo fuente de la tarea Bart

## Tarea TaskHomer

Cree los ficheros `taskhomer.c` y `taskhomer.h` siguiendo la misma estructura que TaskBart. El dibujo de Homer y su comportamiento son análogos: elija un periodo distinto al de Bart y una zona diferente de la pantalla para que ambas figuras sean visibles a la vez.

>[!note] *Archivos de referencia:*
>
> - [taskhomer.h](files_ucosii/taskhomer.h) - Archivo de cabecera para la tarea Homer
> - [taskhomer.c](files_ucosii/taskhomer.c) - Archivo fuente de la tarea Homer

## Registro de las nuevas tareas

Recuerde añadir las nuevas tareas al `main()` con `OSTaskCreateExt()`, definir sus pilas y prioridades, e incluir sus cabeceras en `pract1_rtos.h`.

## Control de errores

µC/OS-II devuelve códigos de error en muchos de sus servicios. Los ficheros de chequeo de errores permiten detectar y mostrar estos errores de forma sencilla:

>[!note] *Archivos de control de errores:*
>
> - [alt_ucosii_simple_error_check.h](files_ucosii/alt_ucosii_simple_error_check.h)
> - [alt_ucosii_simple_error_check.c](files_ucosii/alt_ucosii_simple_error_check.c)

Tras las llamadas a servicios de µC/OS-II que devuelvan un código de error, añada la macro `alt_ucosii_check_return_code(err)` para detectar fallos en tiempo de ejecución. Incluya `stdlib.h` en `pract1_rtos.h`, necesario para el correcto funcionamiento de estos ficheros.

Compile y ejecute. Capture los resultados.

### Cuestiones

- ¿Qué periodo tienen las tareas Bart y Homer? ¿Se ven durante todo el periodo?
- ¿Qué hacen las funciones de chequeo de errores? ¿Dónde las utilizaría?
- ¿Es necesario proteger los accesos VGA de estas tareas con el semáforo? Justifique su respuesta.

[Ir Ejercicio 7](ex7.md)
[Volver a Indice](index.md)
