---
title: "9. Colas de Mensajes. Uso de PS2"
---

## EJERCICIO 9: USO DE KEYBOARD PS2. USO DE COLAS DE MENSAJES

En este ejercicio se pretende introducir un código de tres teclas + INTRO por el teclado PS2.

- Si el código corresponde con un código secreto se visualiza un mensaje **CORRECTO** en la pantalla.
- Si el código es incorrecto se visualiza un mensaje de **INCORRECTO** en la pantalla.

### Preparación inicial

Antes de continuar elimine las tareas 1, 2, 3 y 4 del scheduler eliminando (o comentando) su creación de la función `main()`. Puede usar la `TaskSW` para, mediante los switches, borrarlas con el servicio `OSTaskDel()` si lo hubiera implementado.

### Consideraciones de diseño

Como desarrollador debe pensar que:
- El usuario puede teclear a cualquier velocidad las teclas
- Que puede darse el caso que no pulse INTRO, lo que sería código incorrecto
- Que puede que teclee más de tres teclas, por ejemplo 6 y luego pulse INTRO, esto no puede considerarse CORRECTO
- El número de teclas pulsadas incluyendo INTRO debe ser 4

### Solución recomendada

Para resolver el problema se aconseja:
1. Hacer uso de **interrupciones de teclado PS2**
2. Hacer uso de **colas de mensajes** para enviar la tecla pulsada a la cola
3. Hacer uso de una **Task asíncrona** (p.e. cada segundo) que lea mensajes de cola y verifique que el código es correcto. Debe vaciar la cola si el número de teclas pulsado es 4 (cola llena) o ya se ha detectado el código correcto

### Estudio previo

Para poder programar el código se debe estudiar:
- El funcionamiento de un teclado PS2, incluyendo las teclas de expansión del teclado numérico
- Los servicios asociados a colas de mensajes en UCOSII

> **Nota**: Dedique unos minutos a pensar en cómo programar el ejercicio y discuta con el profesor la solución si está en el laboratorio.

### Implementación de la solución

#### Definición de los elementos de la cola

En `init.c` se declara el puntero tipo evento y vector de 4 punteros para la cola:

```c
OS_EVENT *MessageQueue;
void *MessageQueueTbl[4];
```

Y su creación en `Init_UCOS_Utilities()`:

```c
MessageQueue = OSQCreate(MessageQueueTbl, 4);
```

En `init.h` defina un tipo struct para almacenar varios elementos en la dirección apuntada por la cola, así como el tamaño de esta. Este tipo de estructura nos permite almacenar no solo el valor de la tecla pulsada sino información adicional sobre el momento (time) de su pulsación y el orden (count_key):

```c
/* local structure of data */
#define QUEUE_SIZE 4 
typedef struct msg_data{
  int time;
  int key_pressed;
  int count_key;
}MSG_DATA_PS2;
```

Se debe activar la interrupción de PS2 en la función `Init_App()`:

```c
*(keyboard_ptr + 1) = 0x1; // enable read interrupts on the PS2 controller (RE bit)
alt_irq_register(PS2_KEY_IRQ, NULL, (alt_isr_func) keyboard_isr);
```

#### Productor de elementos de cola (ISR)

En la ISR:
- Se declaran los tipos necesarios, como los 4 punteros `msg_isr` que almacenan las 4 estructuras de valores
- En un diseño avanzado se puede usar una partición de memoria con 4 bloques de 3 palabras y utilizar los servicios de gestión de memoria
- Se chequea si el dato recibido por el teclado PS2 es válido (Bit RVALID del registro DATA) y se lee el byte recibido

### Rutina de atención a interrupciones de teclado PS2
```c
/* PS2 keyboard interrupt service routine */

/*
 * Keyboard ISR. Read key pressed and send it to a queue.
 * Key code is extracted after release a key, so first the code 0xF0 (release) is received.
 * Once the code of release 0xF0 is received then key code is saved on a queue.
 * Size of secret code is 4: 3 numbers or keys and INTRO code (0x5A).
 * If user press more than 4 keys queue is flush and a BAD CODE message is sent.
 *
 */

 void keyboard_isr(){
	OSIntEnter();
  INT8U Keycode;
  INT8U Rvalid;
  INT8U err;
  INT32U ps2_data;
  static	INT8U Release=0;

  static INT8U i=0;
  //local buffer for messages in ISR
  static MSG_DATA_PS2 msg_isr[QUEUE_SIZE];
// static allocates memory only once
// instead of doing it each time the ISR is executed
// which would be inefficient and could lead to fragmentation
// of the memory over time.
// in addition, dynamic memory allocation is not recommended in ISRs
// due to potential delays and unpredictability associated with memory allocation routines.

// Using a static array ensures that memory is allocated at compile time
// and is readily available when the ISR is invoked.
  
	alt_irq_disable(PS2_KEY_IRQ);
	ps2_data=*keyboard_ptr; //read DATA register only once: each read pops a byte from the FIFO
	Rvalid=(ps2_data>>15)&0x01; //check if data is valid (RVALID)
	if (Rvalid){
		Keycode=ps2_data&0xFF; //read the value

		switch (Keycode) {
			case 0xAA: //code received from keyboard when is connected (5V)
				printf("Keyboard Connected...\n");
				Release=0;
				OSTimeSet(0); //reset time counter
				break;
			case 0xf0: //code of release
				Release=1;
				break;
			default:
				if (Release){

					i = (i+1) % QUEUE_SIZE;
					msg_isr[i].time=OSTimeGet();
					msg_isr[i].key_pressed=Keycode;
					msg_isr[i].count_key=i;
					printf("Value on keyboard is %x\n",(INT8U)msg_isr[i].key_pressed);
					err=OSQPost(MessageQueue,(void *)&msg_isr[i]);
					alt_ucosii_check_return_code(err);

					Release=0;
				}
				break;
		}
	}
	else{
		printf("Keyboard data undefined\n");
	}

	alt_irq_enable(PS2_KEY_IRQ);

	OSIntExit();
}
```

Se chequea (switch…case) el código recibido:
- Si es **0xAA** indica teclado conectado (solo lo envía el teclado al inicio cuando recibe 5V)
- Si es **0xF0** una tecla se ha pulsado y soltado (Release)
- Tras un código 0xF0 se debe procesar el siguiente byte recibido que lleva el código definitivo de la tecla pulsada

Se actualizarán los valores recibidos y se postean en la cola:

```c
msg_isr[i].time = OSTimeGet();
msg_isr[i].key_pressed = Keycode;
msg_isr[i].count_key = i;
printf("Value on keyboard is %x\n",(INT8U)msg_isr[i].key_pressed);
err = OSQPost(MessageQueue,(void *)&msg_isr[i]);
```

Se reactiva la interrupción PS2:

```c
alt_irq_enable(PS2_KEY_IRQ);
```

#### Consumidor de datos de la cola (TaskCode)

```c
/*
 * taskcode.c
 *
 *  Created on: 13 de may. de 2024
 *      Author: mpeiro
 */



#include "..\inc\taskcode.h"
#define PRINT_DEBUG

void TaskCode(void* pdata)
{

  MSG_DATA_PS2* msg_rx; //pointer to message received from queue

  static INT8U Count_keys_received=0;

  INT8U error;

  INT32U CodeReceived	=0;
  INT32U SecretCode	=0x69727a5a;  //1(69),2(72),3(7A),INTRO(5A) if keypad used for numbers
  INT32U SecretCode2=0x161e265a;  //1(16),2(1e),3(26),INTRO(5A) if keyboard used for numbers

  while(1)
  {

    OSTimeDlyHMSM(0,0,1,0);

    msg_rx=(MSG_DATA_PS2*)OSQPend(MessageQueue,0,&error);
    alt_ucosii_check_return_code(error);


#ifdef PRINT_DEBUG
    printf("Message received is: Time=%d ms, KeyCode=%x, KeyCount=%d\n",msg_rx->time,msg_rx->key_pressed,msg_rx->count_key);
#endif

    Count_keys_received=Count_keys_received+1;

    CodeReceived=CodeReceived<<8|msg_rx->key_pressed; //creating the 32bits code

    // if INTRO is pressed flush the queue
    if (msg_rx->key_pressed==0x5A){
      OSQFlush(MessageQueue);
      Count_keys_received=0;
    }
#ifdef PRINT_DEBUG
    printf("Secret Code received is %x\n",(int)CodeReceived);
    printf("Count_Keys_received is %x\n",(int)Count_keys_received );
#endif
    //If SecretCode (1-2-3) is received and 4 keys were pressed -> CORRECT CODE
    if (((CodeReceived==SecretCode) || (CodeReceived==SecretCode2))&&(Count_keys_received==0))
    {

      VGA_Clean_Full_Lines(20,23,character_buffer);

      printf("YOU GET THE CODE!!!\n");

      VGA_text(10+15,20,"  OH YES BABY   ",character_buffer);
      VGA_text(8+15,22, "YOU GET THE CODE",character_buffer);

      OSQFlush(MessageQueue);
      CodeReceived=0;
      Count_keys_received=0;

      // uncomment in exercise 10 (EventFlag)
      // OSFlagPost(EventFlag,0x02,OS_FLAG_SET, &error);
      // alt_ucosii_check_return_code(error);

    }
    else
    {

      if (((Count_keys_received<4)&&(msg_rx->key_pressed!=0x5A))){

        VGA_text(10+15,20,"    HEY BABY    ",character_buffer);
        VGA_text(8+15,22, "   WRITING CODES",character_buffer);

      }else{					//even if INTRO is pressed CodeReceived gets 0
        printf("WRONG CODE!!! Code Received: %x\n", (int)CodeReceived);
        CodeReceived=0;
        Count_keys_received=0;
        OSQFlush(MessageQueue);

        VGA_text(10+15,20,"   SORRY BABY   ",character_buffer);
        VGA_text(10+15,22,"   WRONG  CODE  ",character_buffer);

        // uncomment in exercise 10 (EventFlag)
        // OSFlagPost(EventFlag,0x02,OS_FLAG_CLR, &error);
        // alt_ucosii_check_return_code(error);

      }
    }
  }
}
```

En la Task que revisa contenidos de cola (la llamamos `TaskCode`):

1. Se está pendiente de la cola:

```c
msg_rx = (MSG_DATA_PS2*)OSQPend(MessageQueue, 0, &error);
```

2. Se añade el byte del código recibido a las teclas previas recibidas:

```c
CodeReceived = CodeReceived<<8 | msg_rx->key_pressed;
```

3. Si se ha pulsado INTRO se vacía la cola:

```c
if (msg_rx->key_pressed == 0x5A) { // ENTER key
    OSQFlush(MessageQueue);
    Count_keys_received = 0;
}
```

4. Se verifica si el código es correcto y se han pulsado solo 4 teclas y se lanza mensaje a VGA y se vacía la cola

5. Si todavía no se han pulsado las 4 teclas y no se ha pulsado intro se lanza mensaje de "WRITING CODE". Si se han pulsado tres teclas e INTRO y no es correcto se lanza mensaje de error

[Ir Ejercicio 10](ex10.md)
[Volver a Indice](index.md)