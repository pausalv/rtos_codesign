# Análisis del guion de prácticas uC/OS-II

Análisis del guion (`content/*.md` y `content/files_ucosii/`) frente al proyecto terminado `P1_RTOS`, hecho sobre la rama `v4` (2026-10-22). Este fichero está fuera de `content/`, así que Quartz no lo publica.

## Estado en la rama `correccion_guion`

- **Corregidos** (un commit por ejercicio): todos los errores de código de la tabla de la sección 1 (nº 1-16), y además estos puntos de la lista de errores conceptuales:
  - `last_line` de `Print_VGA`;
  - el nombre `Init_UCOS_Utilities()`;
  - el paso 2 del ex5;
  - la explicación desfasada del ex9 y la doble lectura del registro PS/2;
  - el nombre de los ficheros del ex6;
  - el periodo del ex1;
  - el `\"` de `taskhomer.c`;
  - los comentarios de los HEX.
- **Ex11**: se ha incorporado la corrección de `ej11_correccion`:
  - `OSSemCreate(2)` con `OSSemAccept`;
  - bloques de `NUM_BYTES_PACKET` bytes;
  - `OSTaskDel(OS_PRIO_SELF)` cuando no hay bloque libre.

  Además, `OSMemGet` guarda ahora su resultado en `pBlock`, los punteros se imprimen con `%p` y `showpicture.c` declara `err` y añade `#include <string.h>`.
- **Capturas de código** (Fig. 9-22): sustituidas por texto. La Fig. 19 duplicada se ha reemplazado por el código de `Init_UCOS_Utilities()` y se ha borrado `Imagen22_placeholder.txt`. Las figuras del ex11 pasan a ser la 9, la 10 y la 11.
- **Pendiente de decidir**: el `exit` por cola llena en el ex9 (quitar el delay de `TaskCode` o no tratar `OS_Q_FULL` como fatal) y que el guion no define `TASK7_PRIORITY` ni las prioridades del ex6 en adelante.
- **Pendiente** (fase de errores conceptuales y aclaraciones):
  - el resto de la sección 1;
  - toda la sección 3;
  - `Read_BMP_ZipFile` sigue escribiendo directamente en `PixelMem[n]`;
  - los textos del ex11: "cuatro imágenes", on-chip frente a FLASH, 320x240 frente al stride de 512 y el `bool` que no se usa.

Las referencias de línea (`exN.md:NN`) son las de `v4` en el momento del análisis.

---

Gravedad: 🔴 no compila o funciona mal · 🟠 incoherente o confuso · ⚪ estético.

## 1. Errores en el guion

### Errores de código que el alumno va a copiar

| # | Sitio | Problema | Gravedad |
|---|---|---|---|
| 1 | `ex5.md:60` | `OSSemPend(Semaphore, 0, NULL)`. Con `OS_ARG_CHK_EN=1` (tu BSP lo tiene) el kernel hace `if (perr == 0) return;` (`os_sem.c:290`), así que **no coge el semáforo**. No hay exclusión mutua, y cada `OSSemPost` sube el contador a 2, 3… El código "de ejemplo" de la sección crítica no protege nada. | 🔴 |
| 2 | `ex7.md:60` | `OSSemPend(Semaphore);` sin timeout ni `&err`: no compila. | 🔴 |
| 3 | `ex7.md:48` | Usa `TASK_SWITCH_PRIORITY`, que no se define en ningún sitio. El comentario de la línea 30 dice `TASK7_PRIORITY`, que es lo que usas tú. | 🔴 |
| 4 | `ex7.md:59,71` | `if (err != OS_ERR_NONE) printf("resumed/suspended")`: la lógica está invertida y avisa cuando **falla**. Además, dentro del `if` se llama a `OSSemPend(...,&err)`, que machaca el `err` de `OSTaskResume`. | 🔴 |
| 5 | `ex7.md:87` | `*SW_switch_ptr & 0x80` es el **SW7**, no el SW8 (el SW8 es `0x100`). En `ex10.md:97` se usa en cambio `(*SW_switch_ptr>>8)`, que da cierto con SW8 **o** SW9. Son dos versiones distintas y ninguna es exactamente "SW8". Tu proyecto tiene el mismo fallo del `0x80`. | 🔴 |
| 6 | `ex8.md:103-104` | `int *pMailBox_homer = NULL; *pMailBox_homer = homer_pos;` escribe en la dirección 0, que es SDRAM (código o vectores). El texto de `ex8.md:95` explica este error como si fuera la idea correcta. Tu `taskgrinch.c` lo repite. | 🔴 |
| 7 | `ex8.md:66` | La ISR de ex8 ya usa `OSFlagPost(EventFlag,...)`, pero `EventFlag` no se crea hasta el ex10. Si no se declara, no compila. Si se declara pero no se crea, `alt_ucosii_check_return_code` termina el programa al pulsar KEY1. Pasa lo mismo en `TaskCode` del ex9 (`ex9.md:244,265`). | 🔴 |
| 8 | `ex9.md:71` | Registra `isr_ps2_keyboard`, pero la función se llama `keyboard_isr`. | 🔴 |
| 9 | `ex9.md` | **Falta habilitar la interrupción en el controlador PS/2** (`*(keyboard_ptr+1) = 0x1;`, como tienes en tu `init.c:49`). Sin esa línea no llega ninguna interrupción. | 🔴 |
| 10 | `ex10.md:51` | `FlagQuery` no está declarada (falta `OS_FLAGS FlagQuery;`): no compila. | 🔴 |
| 11 | `ex3.md:22` | Define `SW_swith_ptr` (con errata), pero el `.h` y todo lo demás usan `SW_switch_ptr`. Da error de enlazado en cuanto se usa en TaskSW. | 🔴 |
| 12 | `ex3.md:84-91` y Fig. 13 | Los `#include` usan `App_SW/` y `Base_SW/`, pero el ex2 manda crear `AppSW/` y `BaseSW/`. Quien siga el ex2 y copie el ex3 no compila. | 🔴 |
| 13 | `ex11.md` | 2 bloques de memoria, pero `OSSemCreate(4)` ("4 imágenes") y el texto dice "semáforo de nivel 2". Con 4, la 3ª imagen consigue el semáforo, `OSMemGet` falla y el programa termina. Además se escribe en `PixelMem[2..3]`, fuera del array. | 🔴 |
| 14 | `ex11.md:91` | `OSMemCreate(..., NUM_BYTES_PACKET * sizeof(INT8U))`, pero `PixelMem` es de `INT16U`. Para el kernel cada bloque mide la mitad de lo real. | 🔴 |
| 15 | `taskpicture.c:43` | `PixelMem[n][0] = OSMemGet(...)` guarda un puntero en un píxel de 16 bits y nunca usa el bloque que devuelve. Funciona por casualidad, porque la free-list entrega los bloques 0 y 1 en orden. Didácticamente contradice lo que se quiere enseñar. | 🔴 |
| 16 | `showpicture.c:32` | En la rama `UNBLOQUE`, `err` no está declarado. Falta `#include <string.h>` para `memmove`. | 🟠 |

### Errores conceptuales o de texto

- 🔴 **Print_VGA desde la ISR** (`isr.c`, `ex8.md:59-79`): la ISR modifica `line`, la misma variable que se protege con el semáforo en las tareas. Un semáforo no protege frente a una ISR, y la ISR tampoco puede hacer Pend. El ex5 lo plantea al revés ("podría ocurrir cuando una ISR se activa…"), cuando el riesgo real que resuelve el semáforo es que otra tarea expulse a la que está escribiendo.
- 🔴 **Desbordamiento de VGA**: `Print_VGA` llega a `last_line=60` (`files_ucosii/init.c:58`), pero las filas válidas son 0-59. Escribe en la fila 60 y luego borra la 61. Tú lo bajaste a 30.
- 🟠 **Cola llena = programa terminado**: `TaskCode` espera 1 s antes de cada `OSQPend`, y la ISR hace `alt_ucosii_check_return_code` sobre `OSQPost`. Teclear más de 4 teclas en menos de 1 s llena la cola, devuelve `OS_Q_FULL` y se ejecuta `exit(0)`. Tú lo evitaste quitando el delay.
- 🟠 **Nombre de función**: el texto habla siempre de `UCOS_Utilities()` (ex5, ex8, ex9 y el pie de la Fig. 19), pero el `init.c` que se descarga la llama `Init_UCOS_Utilities()`.
- 🟠 **Paso 2 del ex5** (`ex5.md:94`): dice "incluya `init.h` en `pract1_rtos.h"`. Eso ya hacía falta en el ex4 (sin ello no hay `Init_App` ni `line`). Lo que de verdad falta en el ex5 es `isr.h`.
- 🟠 **Explicación del ex9 desfasada** (`ex9.md:278-298`): usa `pKeyboard_msg`, `KEY_MSG`, `Code_saved[i]` y `OSQPend(...,1000,...)`, que no coinciden con el código mostrado justo encima (`msg_rx`, `MSG_DATA_PS2`, desplazamiento de 32 bits, timeout 0). También hay un `\\n` mal escapado en la línea 165.
- 🟠 **Lectura doble del registro PS/2** (`ex9.md:115-117`): se lee `*keyboard_ptr` dos veces. En el core PS/2 de University Program, leer el registro de datos saca el byte del FIFO, así que la segunda lectura puede perder datos. No lo he podido comprobar en placa; conviene leerlo una vez en una variable.
- 🟠 **OSFlagAccept con `WAIT_SET_ALL`** devuelve 0 mientras no estén los 3 flags. El mensaje "Valor de los flags es %x" siempre mostrará 0, no el estado parcial. Para eso haría falta `OSFlagQuery()`. Además, el ex10 dice "bits 1, 2 y 3" y en realidad son los bits 0, 1 y 2.
- 🟠 **ex6**: cita `alt_ucos_error_code.c/.h`, pero los ficheros se llaman `alt_ucosii_simple_error_check.c/.h`.
- 🟠 **Figuras**:
  - **Fig. 19 es la misma imagen que la Fig. 18** (mismo md5). Debería mostrar `UCOS_Utilities()` con el semáforo.
  - El pie de la Fig. 20 dice "funciones de inicio en `main()`", pero la imagen muestra el cuerpo de `Init_App()`.
  - `img/Imagen22_placeholder.txt` sobra y además dice "Figure 23".
- 🟠 **ex1**: la plantilla pone 500 ms en task1/task2 ("~500 ms"), pero la plantilla Hello MicroC/OS-II usa 3 s, igual que las figuras del ex3 y el ex5.
- 🟠 **ex11**: habla de "2 imágenes" y luego de "lee las cuatro imágenes". También dice "memoria on-chip" y después "imágenes de la FLASH". El `bool` que pide definir ya no se usa en `taskpicture.c`.
- 🟠 **ex8.md:134**: "finalice la práctica…" es un resto de cuando el ex8 era el último.
- 🟠 `error` global compartido entre ISR y tareas (ex8 y ex10). Es una condición de carrera; debería ser una variable local.
- ⚪ Faltan tildes en ex3 y ex8-10 ("Indice", "codigo", "perifericos"). El índice dice "Funciones Reentrantes" y el título del ex4 "No Reentrantes". En el ex6 hay viñetas con `•` en lugar de lista markdown.
- ⚪ En `taskhomer.c:22`, `"     ."      ".         "` son dos literales que el compilador concatena, así que el dibujo pierde las comillas. Habría que escapar con `\"`.
- ⚪ En `ex3.md:24-25` los comentarios dicen "HEX3_HEX0" y "HEX5_HEX4", que no coinciden con los nombres de los punteros. En la ISR del ex8 queda el comentario "post the semaphore for the corresponding task".

---

## 2. Tu proyecto frente al guion

| Diferencia | Tipo |
|---|---|
| Nadal/Grinch → Bart/Homer. Grinch tenía periodo 4 s y el mailbox activo; Homer tiene 7 s y el mailbox comentado hasta el ex8. | Sustancial pero esperada. El periodo de 7 s choca con "cada 2 segundos" del ex8. |
| Las ISR (`pushbutton_isr`, `keyboard_isr`) están en tu `init.c`; en el guion van en `isr.c`/`isr.h`. | **Reubicación** |
| Llamas a `UCOS_Utilities()` desde `main` después de `Init_App()`; el guion llama a `Init_UCOS_Utilities()` dentro de `Init_App()`. | **Reubicación.** En ambos casos las ISR se registran antes de crear los objetos del kernel: pequeña ventana de riesgo. |
| Mensaje del mailbox: global en tu `init.c`; `static` local en la ISR del guion. | **Reubicación** |
| Cola: `Queue[]`/`MSG_DATA`/`msg_isr` global en `init.h` (tuyo) frente a `MessageQueueTbl`/`MSG_DATA_PS2`/`static` en la ISR (guion). | **Renombrado/reubicación** |
| Tú defines variables globales en los `.h` (`init.h`, `pract1_rtos.h` sin `extern`); el guion usa `extern` correctamente. | Reubicación, aunque lo tuyo depende de `-fcommon` y rompería con GCC ≥10. |
| ISR de pulsadores: tú usas `switch` por valor exacto (no admite dos teclas a la vez), no llamas a Print_VGA, haces Post solo con KEY2/3 y saturas en 2..15. El guion usa `if` por bits, llama a Print_VGA en la ISR, hace Post siempre y satura en 3..45. | **Sustancial** en comportamiento; funcionalmente equivalente. |
| `Print_VGA`: `last_line` = 30 (tuyo) frente a 60 (guion). | **Sustancial** (el guion se sale de rango) |
| `led.c`: en el tuyo, `Led_ON_Some(LED_ptr,8,2)` **no hace nada** (`init+n > 9` lo rechaza) y `Led_*_All` no llega al LED9. La versión del guion está corregida. | **Sustancial** (el fallo es tuyo; el guion lo arregla) |
| `vga.c` del guion añade comprobación de límites en `VGA_text`/`VGA_box`; el tuyo no la tiene. | Mejora del guion |
| PS/2: tú habilitas la interrupción en el controlador (`keyboard_ptr+1`); el guion no. | **Sustancial** (le falta al guion) |
| `TaskCode`: tú quitaste el delay de 1 s (Pend bloqueante). Cambian también columnas y borrado de VGA y los textos. | **Sustancial** (evita el `exit` por cola llena) + estético |
| `TaskFlags`: tú declaras `FlagQuery`/`err` locales y no usas semáforo. El guion usa el `error` global, `FlagQuery` sin declarar y semáforos. | **Sustancial** (el guion no compila) |
| `TaskSW`: tú no usas semáforo en los `printf`, usas `TASK7_PRIORITY` y tienes el flag del SW activo. El guion usa semáforos, `TASK_SWITCH_PRIORITY` y el flag comentado. Ninguno de los dos implementa `OSTaskDel`. | Mezcla: la parte de semáforos es una mejora; la de prioridad es un fallo del guion. |
| Tareas 1-4: tú usas `&err` con comprobación (como la Fig. 21) y sin número de línea. El guion usa `snprintf` con número de línea y `NULL`. | Estético, pero el `NULL` del guion es el fallo nº 1. |
| Prioridades: tú tienes TASK1..11 en `pract1_rtos.h` (Bart/Homer 5/6, SW 7, Code 8, Flags 9). El guion **no asigna prioridades** a las tareas del ex6 en adelante. | Hueco del guion |
| Ex11 (TaskPicture/ShowPicture) **no está en tu proyecto**. No he podido compararlo con una versión que funcione. | — |
| `alt_ucosii_simple_error_check` está en tu `BaseSW`; el guion no dice dónde va. El contenido es idéntico salvo los saltos de línea. | Reubicación |
| Restos de MTL (`mtl.h` incluido en `init.h`), textos "2025/26", caja negra `320` frente a `320-1`. | Estético |

---

## 3. Lo que falta explicar o habría que aclarar

1. **Dónde y cómo se crea cada tarea nueva** (del ex6 al ex11): pila, `TASKx_PRIORITY`, `OSTaskCreateExt` en `main` y qué `.h` añadir a `pract1_rtos.h`. El guion lo da por hecho a partir del ex6.
2. **Orden de creación de los objetos del kernel**: conviene crear `Semaphore`, `SharedMail`, `MessageQueue` y `EventFlag` en `UCOS_Utilities()` **antes** de registrar las ISR. También hay que decir explícitamente en qué fichero se declaran, dónde va el `extern` y en qué ejercicio aparece cada uno.
3. **Registros de los periféricos**: qué son `KEY_ptr+2` (máscara de interrupción) y `KEY_ptr+3` (edge capture), y cuáles son los registros de datos y de control del PS/2 (RVALID en el bit 15, RE en el bit 0 del registro de control).
4. **OSIntEnter/OSIntExit**: el HAL de Nios ya los llama en `alt_irq_handler.c:95` (`ALT_OS_INT_ENTER`). Hay que aclarar que llamarlos también en la ISR es redundante (solo anida el contador), o explicar por qué se pide.
5. **Unidades de tiempo**: `OS_TICKS_PER_SEC = 1000` en este BSP. Sin ese dato no se puede responder "¿qué representa el 1000 en `OSMboxPend`?". También hay que decir que con `OSTimeDly` el periodo es aproximado (retardo + ejecución + timeout del pend). Esto afecta al "Homer cada 2 s" del ex8, que no indica qué retardo poner en lugar de los 7 s.
6. **Qué hacer en una ISR y qué no**: no se puede hacer Pend, `printf` desde una ISR es mala práctica (JTAG UART lento y que bloquea), y no se pueden compartir variables con tareas protegiéndolas solo con un semáforo.
7. **Mailbox de puntero**: se pasa la dirección de una variable `static`, no su valor. Falta explicar qué pasa si el mailbox está lleno (`OS_MBOX_FULL` sin comprobar) y por qué el valor puede cambiar antes de que Homer lo lea.
8. **Configuración del BSP**: `OS_MAX_TASKS=10` (sin contar idle y stat), `OS_LOWEST_PRIO=20`, `OS_ARG_CHK_EN`, y que hay que tener activados mutex, flags, mem y Q. Para el mutex, explicar que el PIP debe ser una prioridad libre y más alta que la de todas las tareas que lo usan. Esto también responde la pregunta del ex5.
9. **Recursos compartidos que no se protegen**: Bart, Homer, TaskCode y TaskFlags escriben en la VGA y hacen `printf` sin semáforo, y además se pisan zonas de pantalla: Bart y Homer borran las columnas 40-73 de las líneas 3-59, que incluyen la línea 50 de TaskFlags. Estaría bien un mapa de pantalla que diga qué tarea usa qué zona.
10. **Interacción de los LEDs**: Bart enciende todos, Homer los apaga y las tareas 1-4 los conmutan. Conviene avisar para que el alumno no lo tome por un fallo.
11. **Suspender o borrar una tarea que tiene el semáforo cogido** provoca un bloqueo (deadlock) del resto de tareas. Es una pregunta de reflexión natural para el ex7 que no está. El comentario del código ("11:none") también tiene que cambiar cuando se añade `OSTaskDel`.
12. **Teclas extendidas del PS/2** (`E0`): se pide estudiarlas, pero no se explica cómo las trata la ISR. Tampoco se explica que `count_key` es el índice del buffer circular y no el orden de pulsación.
13. **Ex11**: falta explicar:
    - el stride de 512 (`offset = (row<<9)+col`), que obliga a bloques de 512×240×2 bytes y no de 320×240;
    - quién graba los ficheros en la memoria de `0x0408_0000` (¿van en el `.sof`?);
    - que `ShowPicture` empieza a mostrar antes de que haya imagen cargada.
14. **Portabilidad de los `#include` con `\`**: solo funcionan en Windows. Si alguien compila en Linux (como tú ahora) debe usar `/`.
15. **Firmas de las tareas**: `TaskCode()` y `TaskFlags()` no tienen `void* pdata`, lo que da un warning de tipo incompatible en `OSTaskCreateExt`. Lo mismo pasa al pasar `pushbutton_isr` sin argumentos a `alt_irq_register`, que espera `(void*, alt_u32)`.

---

**Otra cosa:** `content/LabSolution/P1_RTOS_25_MMP/` está dentro de `content/`, y `ignorePatterns` en `quartz.config.ts:20` no la excluye. Si eso es la solución del profesor, Quartz la publicaría en la web junto con el guion.
