---
title: "6. Tareas Bart y Homer"
---

En esta práctica trabajaremos con unos dibujos de Bart y Homer en la pantalla VGA.

El objetivo de este ejercicio es crear dos tareas periódicas de dibujo:

- `TaskBart`: dibuja la figura de Bart en la pantalla.
- `TaskHomer`: dibuja la figura de Homer en la pantalla.

Implemente ambas tareas usando los archivos proporcionados, intégralas en el proyecto RTOS, compile y capture los resultados de ejecución.

### Tarea taskbart

Construya la tarea `TaskBart` con el siguiente código y verifique que el dibujo se visualiza correctamente en VGA con su periodo configurado.
```c
void TaskBart(void* pdata)
{
	char visualiza_string[40] = "Hey Man!!";
	char blank[36]="                                  \0";

	char visualiza_string_bart[15][40]=
	{					"                 ",
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
            "                 "};

	int bart_pos=10;
  while (1)
  {
	  printf("%s\n",visualiza_string);

	  OSTimeDlyHMSM(0, 0, 4, 0);
    Led_ON_All(LED_ptr);

    //clean text on black box
    for (int var = 3; var < 60; ++var) {
      VGA_text (40, var, blank,character_buffer);
    }

    for (int var = 0; var < 15; ++var) {
      VGA_text (50, bart_pos+var, visualiza_string_bart[var],character_buffer);
    }
  }
}
```

### Tarea taskhomer

Construya la tarea `TaskHomer` y compruebe su visualización y sincronización temporal con el resto de tareas del sistema.

>[!note] *Archivos para descargar:*
>
> - [taskbart.h](files_ucosii/taskbart.h) - Archivo de cabecera para la tarea Bart
> - [taskbart.c](files_ucosii/taskbart.c) - Archivo fuente de la tarea Bart
> - [taskhomer.h](files_ucosii/taskhomer.h) - Archivo de cabecera para la tarea Homer
> - [taskhomer.c](files_ucosii/taskhomer.c) - Archivo fuente de la tarea Homer

Puede usar las funciones de chequeo de errores `alt_ucosii_simple_error_check.c` y su fichero de cabecera `alt_ucosii_simple_error_check.h` tras los códigos de uso de servicios de UCOS que proporcionen variables de error como retorno de llamadas al servicio.


>[!note] *Archivos de control de errores:*
>
> - [alt_ucosii_simple_error_check.h](files_ucosii/alt_ucosii_simple_error_check.h) - Archivo de cabecera para control de errores UCOS
> - [alt_ucosii_simple_error_check.c](files_ucosii/alt_ucosii_simple_error_check.c) - Archivo fuente para control de errores UCOS

Incluya la librería stdlib.h en pract1_rtos.h, necesaria para el correcto funcionamiento de `alt_ucosii_simple_error_check.c`
•	¿Qué periodo tienen las tareas Bart y Homer? ¿Se ven durante todo el periodo?
•	¿Qué hacen las funciones de chequeo de errores? ¿dónde las utilizaría?

[Ir Ejercicio 7](ex7.md)
[Volver a Indice](index.md)
