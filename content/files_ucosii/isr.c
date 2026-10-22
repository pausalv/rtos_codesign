/* atencion a interrupcion de pulsadores KEY en la DE1Soc */

#include "..\inc\isr.h"

void pushbutton_isr()
{
OSIntEnter(); /* Inform the OS that we are starting an ISR */

  char vga_msg[60];
    
  INT8U key_pressed;

  /* read the KEY interrupt register to determine which KEY was pressed */
  key_pressed = *(KEY_ptr + 3);   

  /* clear the interrupt */
  *(KEY_ptr + 3) = key_pressed;    

  /* check which KEY was pressed */
  if (key_pressed & 0x1)   // KEY0 pressed
  {
    snprintf(vga_msg, sizeof(vga_msg), "%02d   KEY0 Interruption", line);
    Print_VGA(vga_msg, &line);
  }
  if (key_pressed & 0x2)   // KEY1 pressed
  {
    snprintf(vga_msg, sizeof(vga_msg), "%02d   KEY1 Interruption", line);
    Print_VGA(vga_msg, &line);
  }
  if (key_pressed & 0x4)   // KEY2 pressed
  {
    snprintf(vga_msg, sizeof(vga_msg), "%02d   KEY2 Interruption", line);
    Print_VGA(vga_msg, &line);
  }
  if (key_pressed & 0x8)   // KEY3 pressed
  {
    snprintf(vga_msg, sizeof(vga_msg), "%02d   KEY3 Interruption", line);
    Print_VGA(vga_msg, &line);
  }

OSIntExit(); /* Inform the OS that we are leaving the ISR */
}