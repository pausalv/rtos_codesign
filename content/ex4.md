---
title: "4. Uso de VGA y Funciones No Reentrantes"
---

## Contexto y objetivo

Hasta ahora hemos enviado mensajes por UART mediante `printf`. En este ejercicio incorporamos la pantalla VGA como segundo canal de salida. Para ello crearemos una función de inicialización `Init_App()` que configure todos los periféricos antes de que arranque el scheduler.

Sin embargo, este ejercicio tiene un segundo objetivo igual de importante: introducir el concepto de **función no reentrante** y reflexionar sobre los problemas que plantea en un RTOS. Una función es no reentrante cuando su ejecución simultánea desde varios contextos puede producir resultados incorrectos, normalmente porque manipula datos compartidos (variables globales, buffers estáticos). En un RTOS, cualquier tarea puede ser interrumpida en cualquier punto y reemplazada por otra de mayor prioridad, lo que hace que las funciones no reentrantes sean especialmente peligrosas.

## Archivos de inicialización

Añadiremos una función `Init_App()` en un fichero fuente `init.c`, ubicado en `AppSW/src`. Su correspondiente `init.h` irá en `AppSW/inc`.

>[!note] *Archivos para descargar:*
>
> - [init.h](files_ucosii/init.h) - Archivo de cabecera con prototipos y variables globales
> - [init.c](files_ucosii/init.c) - Archivo fuente con implementación de Init_App()

Estudie el contenido de ambos ficheros. El fichero `init.h` hace públicas las variables globales definidas en `init.c` así como los prototipos de sus funciones.

<div align="center">
	<img src="img/Imagen16.png" alt="Ejemplo de init.h" width="450"/>
	<br>
	<em>Figura 16. Ejemplo de init.h</em>
</div>

<br>

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
