---
title: "2. Estructuración del Proyecto Software"
---

## Contexto y objetivo

A medida que un proyecto crece, concentrar todo el código en un único fichero se vuelve inmanejable. En sistemas embebidos es habitual separar el código en capas funcionales: una **capa de aplicación** con la lógica de las tareas y una **capa de soporte hardware** (*Base Software*) que encapsula el acceso directo a los periféricos.

Esta separación tiene ventajas concretas: el driver de un periférico puede reutilizarse en otros proyectos sin modificaciones; la lógica de una tarea puede cambiarse sin tocar el hardware; y el proyecto resulta más fácil de depurar porque cada fichero tiene una responsabilidad única. En este ejercicio reorganizamos el proyecto en esa estructura, moviendo cada tarea a su propio par de ficheros `.c`/`.h`.

Es fundamental aprovechar las ventajas de un RTOS mediante la programación de tareas en ficheros fuente individuales.

### Capa de aplicación

Los elementos no dependientes del hardware se ubicarán en la carpeta `AppSW`, y los ficheros independientes del hardware pero más cercanos a él se ubicarán en una carpeta llamada `BaseSW`.

Dentro de cada carpeta se crearán ficheros fuente `.c` y cabecera `.h`, organizados en estas subcarpetas:

- `AppSW/inc`
- `AppSW/src`
- `BaseSW/inc`
- `BaseSW/src`

Cree las carpetas indicadas mediante `File -> New -> Source Folder` y `Folder` para las carpetas `src` e `inc`. También puede crearlas desde un editor como VS Code.

<div align="center">
	<img src="img/Imagen5.png" alt="Creación de carpetas y subcarpetas desde Eclipse" width="600"/>
	<br>
	<em>Figura 5. Creación de carpetas y subcarpetas desde Eclipse. Creación de ficheros fuente y cabecera.</em>
</div>
<br>

Una vez creadas las carpetas observará un árbol como el mostrado en la Figura 6.

<div align="center">
	<img src="img/Imagen6.png" alt="Árbol de carpetas del proyecto software" width="190"/>
	<br>
	<em>Figura 6. Árbol de carpetas del proyecto software tras crear AppSW y BaseSW.</em>
</div>

<br>

Incluya en `BaseSW/inc` el fichero `led.h` y en `BaseSW/src` el fichero `led.c` entregado en la práctica. Realice lo mismo con el fichero de VGA.

>[!note] *Archivos para descargar:*
>
> - [led.h](files_ucosii/led.h) - Archivo de cabecera para el control de LEDs
> - [led.c](files_ucosii/led.c) - Archivo fuente para el control de LEDs  
> - [vga.h](files_ucosii/vga.h) - Archivo de cabecera para el control de VGA
> - [vga.c](files_ucosii/vga.c) - Archivo fuente para el control de VGA

<div align="center">
	<img src="img/Imagen7.png" alt="Carpetas de BaseSW con ficheros de control de periféricos" width="190"/>
	<br>
	<em>Figura 7. Carpetas de BaseSW con los ficheros de control de periféricos.</em>
</div>

<br>

Observe el contenido de los ficheros `.c` y `.h` entregados.

Edite el contenido del programa principal `pract1_rtos.c` para que no aparezcan los cuerpos de las tareas, sino que estos se localicen en ficheros independientes. Para ello deberá crear `task1.c` y `task1.h` y almacenarlos en las carpetas `AppSW/src` y `AppSW/inc`. Haga lo mismo con el resto de tareas.

<div align="center">
	<img src="img/Imagen8.png" alt="Carpeta AppSW/inc con sus ficheros de cabecera" width="190"/>
	<br>
	<em>Figura 8. Carpeta AppSW/inc con sus ficheros de cabecera.</em>
</div>

<br>

Un ejemplo de `task1.c` en sus respectivos directorios es:

<div align="center">
	<img src="img/Imagen9.png" alt="Fichero task1.c con la inclusión de cabecera" width="600"/>
	<br>
	<em>Figura 9. Fichero task1.c con la inclusión de cabecera.</em>
</div>

<br>

<div align="center">
	<img src="img/Imagen10.png" alt="Fichero task.h con la inclusión de librería principal pract1_rtos.h" width="450"/>
	<br>
	<em>Figura 10. Fichero task.h con la inclusión de librería principal pract1_rtos.h.</em>
</div>

<br>

Para poder crear las tareas desde el fichero principal, necesita incluir ahora los ficheros de cabecera de las tareas; de lo contrario no localizará el cuerpo de las tareas al utilizar el servicio `OSTaskCreateExt()`. Una forma elegante es incluir solo el fichero `pract1_rtos.h` en `pract1_rtos.c` y, en ese fichero de cabecera, incluir los elementos indicados en la Figura 11.

<div align="center">
	<img src="img/Imagen11.png" alt="Inclusión de ficheros de cabecera en pract1_rtos.h" width="380"/>
	<br>
	<em>Figura 11. Inclusión de ficheros de cabecera en fichero pract1_rtos.h.</em>
</div>

<br>

Compile y verifique que el proyecto sigue funcionando correctamente, pero ahora con una estructura modular.

### Cuestiones

- ¿Para qué es útil estructurar las carpetas en capas?
- ¿Qué ficheros de cabecera deben incluirse en el fichero fuente `.c` de cada tarea?
- ¿Qué se declara en los ficheros de cabecera `.h` en C?

[Ir Ejercicio 3](ex3.md)
[Volver a Índice](index.md)
