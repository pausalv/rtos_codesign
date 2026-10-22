---
title: "5. Sección Crítica: Semáforos e Interrupciones"
---

Ahora pasamos a ver cómo mejorar el sistema para que no se pueda utilizar la función no reentrante `Print_VGA()` por varias task al mismo tiempo. Suponiendo que una task es expulsada por otra cuando está en esta función las consecuencias pueden ser nefastas ya que la actualización de la variable global compartida no es segura. Ello podría ocurrir cuando una ISR se activa en el instante en que la task intenta usar `Print_VGA()`.

Una manera de asegurar el uso de la variable compartida es considerar el acceso a la función `Print_VGA()` como una **sección crítica** dentro de cada task y utilizar **semáforos**.

### Creación de semáforos
Para ello creamos un semáforo:
 
1. En `init.c` declaramos el puntero `OS_EVENT *Semaphore;` (descomente las líneas de declaración de semáforo) y creamos una función para creación de elementos de comunicación de tareas en UCOS (elimine comentarios para que se ejecute el servicio de creación de semáforos).
2. En `init.h` elimine el comentario para que se publique el semáforo creado.

<div align="center">
	<img src="img/Imagen19.png" alt="Creación de función Init_UCOS_Utilities()" width="500"/>
	<br>
	<em>Figura 19. Creación de función Init_UCOS_Utilities().</em>
</div>

<br>

`Init_UCOS_Utilities()` forma parte de la inicialización del sistema y se ejecuta antes de crear las tasks al ser llamada en `Init_App()`.

<div align="center">
	<img src="img/Imagen20.png" alt="Inclusión de funciones de inicio en main()" width="500"/>
	<br>
	<em>Figura 20. Inclusión de funciones de inicio en main().</em>
</div>

<br>

Así podemos usar el semáforo en todas las tareas que compartan la variable global "`line`".

<div align="center">
	<img src="img/Imagen21.png" alt="Uso de semáforo binario en una task" width="500"/>
	<br>
	<em>Figura 21. Uso de semáforo binario en una task.</em>
</div>

<br>

### Task1 con uso de semáforos para el uso de UART JTAG y VGA
```c
/* include necessary headers */

#include "..\inc\task1.h"


/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{

  INT8U err;
  char vga_msg[60];
  while (1)
  { 
    Toggle_Led(LED_ptr,0);
    
    /* critical section */
    OSSemPend(Semaphore, 0, &err);

    printf("Hello from task1\n");
    snprintf(vga_msg, sizeof(vga_msg), "%02d   Hello from task1", line);
    Print_VGA(vga_msg, &line);
    
    OSSemPost(Semaphore);
    /* end of critical section */

    Toggle_Led(LED_ptr,0);
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}
```

Utilícelo en todas las tareas que compartan la variable `line`. Compile y verifique el resultado.

**Preguntas de reflexión:**
- ¿Por qué se usa un semáforo binario y no uno de valor N?
- ¿Podría utilizarse un MUTEX?
- ¿Qué servicios de UCOS utilizaría con un MUTEX?
- ¿Cuál sería la prioridad de herencia que crearía en este caso al usar un MUTEX?

### Interrupciones de pulsadores

Ahora vamos a incluir interrupciones de pulsadores en nuestro sistema. Para ello copie los ficheros `isr.c` e `isr.h` entregados en la práctica en su carpeta `AppSW`. Estudie el contenido de ambos.

>[!note] *Descarga ficheros de atención a interrupción*
>
> - [isr.h](files_ucosii/isr.h) - Archivo de cabecera para rutinas de servicio de interrupción
> - [isr.c](files_ucosii/isr.c) - Archivo fuente con implementación de ISR para pulsadores

**Pasos a seguir:**
1. Active la habilitación de interrupciones de pulsadores eliminando los comentarios oportunos en el fichero `init.c`
2. Incluya la librería `isr.h` en la inclusión de librerías de `pract1_rtos.h`
3. Compile y ejecute sobre NIOSII.

**Pregunta de reflexión:**
- ¿Qué servicios se utilizan para incorporar la ISR al scheduler de UCOSII?

[Ir Ejercicio 6](ex6.md)
[Volver a Indice](index.md)