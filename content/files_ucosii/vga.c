#include "system.h"
#include <stdio.h>

#define HORIZONTAL_CHARACTERS 80
#define VERTICAL_CHARACTERS 60
#define VGA_TEXT_BUFFER_SIZE (HORIZONTAL_CHARACTERS * VERTICAL_CHARACTERS)
// int main_rcolom(void)
// {
// 	/* mensaje a visualizar en la VGA */
// 	char text_top_row[40] = "Altera DE1-SoC\0";
// 	char text_bottom_row[40] = "Media Computer\0";

// 	/* crea el texto en medio del monitor VGA */
// 	VGA_text (32, 28, text_top_row);
// 	VGA_text (32, 29, text_bottom_row);
// 	VGA_box (0, 0, 80*4-1, 60*4-1, 0x0000); // Pinta de Negro toda la pantalla
// 	VGA_box (31*4, 27*4, 47*4, 31*4, 0x001F); //Dibuja un cuadro azul en el centro

// }


/****************************************************************************************
 * Subrutina para enviar una cadena de texto a la pantalla VGA
****************************************************************************************/
void VGA_text(int x, int y, char * text_ptr, volatile char *character_buffer)
{
	int offset;
	int i, len = 0;
	char safe_buf[HORIZONTAL_CHARACTERS + 1];

	/* comprobar rango x e y */
	if (x < 0 || x >= HORIZONTAL_CHARACTERS) return;
	if (y < 0 || y >= VERTICAL_CHARACTERS) return;

	/* calcular longitud de la cadena (limitada a HORIZONTAL_CHARACTERS) */
	while (text_ptr[len] != '\0' && len < HORIZONTAL_CHARACTERS) ++len;

	/* ajustar si la cadena se sale de la pantalla */
	if (x + len > HORIZONTAL_CHARACTERS) len = HORIZONTAL_CHARACTERS - x;

	/* copiar a un buffer seguro y apuntar text_ptr a él */
	for (i = 0; i < len; ++i) safe_buf[i] = text_ptr[i];
	safe_buf[len] = '\0';
	text_ptr = safe_buf;

  	/* asume que la cadena de texto comienza en la primera fila */
	offset = (y << 7) + x;
	while ( *(text_ptr) )
	{
		*(character_buffer + offset) = *(text_ptr);	// escribe en el buffer
		++text_ptr;
		++offset;
	}
}

/****************************************************************************************
 * Dibujar un rectangulo en la pantalla VGA
****************************************************************************************/
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color, volatile short *pixel_buffer)
{
	int offset, row, col;
	// int SDRAM_BASE_SIN_CACHE = (SDRAM_BASE + VIDEO_SDRAM_BASE + NIOS2_DCACHE_BYPASS_MASK);
  	// volatile short * pixel_buffer = (short *) SDRAM_BASE_SIN_CACHE;	// VGA pixel buffer
	/* Limitar coordenadas a 320x240 */
	#define VGA_PIXELS_X 320
	#define VGA_PIXELS_Y 240

	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 >= VGA_PIXELS_X) x2 = VGA_PIXELS_X - 1;
	if (y2 >= VGA_PIXELS_Y) y2 = VGA_PIXELS_Y - 1;

	/* Asegurar orden correcto de coordenadas */
	if (x2 < x1) { int tmp = x1; x1 = x2; x2 = tmp; }
	if (y2 < y1) { int tmp = y1; y1 = y2; y2 = tmp; }

	/* Asegurar que la suma x2-x1 no sobrepase el ancho y que y2-y1 no sobrepase la altura.
		Si x2 o y2 se pasan como tamaño (en lugar de coord. absoluta), los ajusta para que no desborden. */
	if ((x2 - x1) >= VGA_PIXELS_X) x2 = VGA_PIXELS_X - 1;
	if ((y2 - y1) >= VGA_PIXELS_Y) y2 = VGA_PIXELS_Y - 1;	


	/* Dibujar el rectangulo */
	for (row = y1; row <= y2; row++)
	{
		col = x1;
		while (col <= x2)
		{
			offset = (row << 9) + col;
			*(pixel_buffer + offset) = pixel_color;	//procesa mitad direcciones
			++col;
		}
	}
}

/**
 *  @brief Funcion para borrar lineas de VGA
 *  @param half_all 0->borra mitad de linea, 1-> borra linea completa
 *  @param inicio indica linea inicial de borrado
 *  @param lineas indica numero de lineas a borrar desde inicial
 *  @param character_buffer puntero del periferico vga para borrado
 */
void VGA_Clean_Lines(int inicio, int lineas, volatile char *character_buffer)
{
	int i;
	char blank_half[HORIZONTAL_CHARACTERS/2]="                          \0";
	for (i=inicio;i<lineas;i++)
	{
		VGA_text(0,i,blank_half,character_buffer);
	}

}

void VGA_Clean_Full_Lines(int inicio, int lineas, volatile char *character_buffer)
{
	int i;
	char blank_all[HORIZONTAL_CHARACTERS]="                                                              \0";
	for (i=inicio;i<lineas;i++)
	{
		VGA_text(0,i,blank_all,character_buffer);
	}

}
