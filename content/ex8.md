---
title: "8. Mailbox. Uso de Interrupciones"
---

Vamos a modificar la rutina de atención a interrupción de los pulsadores KEY. Recuerde que está manejada por el scheduler mediante el uso de los servicios `OSIntEnter()` y `OSIntExit()`.

### Funcionalidad de la interrupción

La interrupción ahora realizará la siguiente función: 
- **Pulsando el botón KEY3**: el dibujo de Homer sube de posiciones 
- **Pulsando el botón KEY2**: el dibujo desciende

Para conocer la nueva posición donde se pintará el dibujo se debe comunicar mediante un paso de mensajes con **MailBox**. Los MailBox no necesitan usar variables compartidas entre tasks, tan solo deben escribir un valor en el mail desde ISR o tarea y leer el nuevo valor desde otra tarea.

### Creación del MailBox

1. Cree el puntero para el mailbox `OS_EVENT *SharedMail;` en el fichero `init.c` y publíquelo en `init.h`

2. Cree el mailbox dentro de la función `Init_UCOS_Utilities()`:
   ```c
   SharedMail = OSMboxCreate(NULL);
   ```

### Envío de MailBox (post) desde ISR

Cree la rutina de atención a la interrupción de pulsadores y utilice los servicios de MailBox para enviar un mensaje y para recibir un mensaje. Aprecie la creación de variable y puntero locales en la subrutina para el envío de la nueva posición al MailBox.

<br>

### Modificacion de ISR para incluir los envíos a MailBox (Post) de la nueva posición de la imagen

```c
/* atencion a interrupcion de pulsadores KEY en la DE1Soc */

#include "..\inc\isr.h"

void pushbutton_isr()
{
OSIntEnter(); /* Inform the OS that we are starting an ISR */

  char vga_msg[60];
    
  INT8U key_pressed;

  //local variable to store position message for ISR
  static int PositionMessage_isr=10;
  int * pMessage_isr= &PositionMessage_isr;
  

  /* read the KEY interrupt register to determine which KEY was pressed */
  key_pressed = *(KEY_ptr + 3);   

  /* clear the interrupt */
  *(KEY_ptr + 3) = key_pressed;    

  /* post the semaphore for the corresponding task */
  if (key_pressed & 0x1)   // KEY0 pressed
  {
    snprintf(vga_msg, sizeof(vga_msg), "%02d   KEY0 Interruption", line);
    Print_VGA(vga_msg, &line);
  }
  if (key_pressed & 0x2)   // KEY1 pressed
  {
    snprintf(vga_msg, sizeof(vga_msg), "%02d   KEY1 Interruption", line);
    Print_VGA(vga_msg, &line);
    OSFlagPost(EventFlag,0x01,OS_FLAG_SET, &error);
    alt_ucosii_check_return_code(error);
  }
  if (key_pressed & 0x4)   // KEY2 pressed
  {
    PositionMessage_isr+=1;
    snprintf(vga_msg, sizeof(vga_msg), "%02d   KEY2 Interruption", line);
    Print_VGA(vga_msg, &line);
  }
  if (key_pressed & 0x8)   // KEY3 pressed
  {
    PositionMessage_isr-=1;
    snprintf(vga_msg, sizeof(vga_msg), "%02d   KEY3 Interruption", line);
    Print_VGA(vga_msg, &line);
  }

  //saturation of PositionMessage to avoid writing outside VGA limits
  if (PositionMessage_isr<3) PositionMessage_isr=3;
  if (PositionMessage_isr>45) PositionMessage_isr=45;
  
  OSMboxPost(SharedMail, (void *) pMessage_isr);

OSIntExit(); /* Inform the OS that we are leaving the ISR */
}
```
### Recepción del MailBox (Pend) desde tarea Homer

Modifique la tarea Homer para que se dibuje cada 2 segundos en la posición enviada desde la ISR. Para ello en la tarea debe recibir la nueva posición desde el MailBox y modificar el valor de la variable que indica desde donde se inicia el dibujo: `homer_pos`.

Antes del bucle de la task declaramos un puntero para almacenar los mensajes recibidos y le damos un valor inicial a su contenido de 10 para que, aunque no se pulse ninguna KEY, se siga dibujando en la línea 10.

<br>

### declaración de puntero local al mailbox
```c
/* local pointer declaration*/

  int *pMailBox_homer = NULL;
    *pMailBox_homer = homer_pos;
```

Dentro del bucle recibimos en la dirección `pMailBox_homer` el mensaje desde el mailbox `SharedMail`.

<br>

### recepción del mailbox en el bucle
```c
//receive message from mailbox

    pMailBox_homer = (int*) OSMboxPend(SharedMail,1000,&err);
    if(err == OS_ERR_NONE){
      homer_pos = *pMailBox_homer;
      OSSemPend(Semaphore,0,&err);
      alt_ucosii_check_return_code(err);
      printf("Mail is %d: new start position is %d\n",*pMailBox_homer,homer_pos);
      OSSemPost(Semaphore);
      alt_ucosii_check_return_code(err);
    }
```

Compile y verifique el funcionamiento mediante la pulsación de los KEY apropiados.

**Preguntas de reflexión:**
- ¿Qué diferencias existen entre semáforos y mailbox?
- ¿Qué ventajas puede presentar un mailbox?
- ¿Qué representa el valor 1000 en el servicio `OSMboxPend()`?
- ¿Podemos recibir caracteres ASCII por un mailbox?

Una vez desarrollada la solución muéstrela al profesor y finalice la práctica mediante el archivado del proyecto software en un .ZIP desde eclipse.

[Ir Ejercicio 9](ex9.md)
[Volver a Indice](index.md)