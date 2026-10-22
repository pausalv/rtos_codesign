/*
 * taskpicture.c
 *
 *  Created on: 20 de may. de 2024
 *      Author: mpeiro
 */



#include "..\inc\taskpicture.h"




	static int filezip_number=0;



void TaskPicture(void* pdata)
{

	INT8U err;

	while(1){


	printf("Hello from Picture\n");

	OSMutexPend(MutexMemory,0,&err);

		sprintf(filezipname, "/mnt/rozipfs/imagen%02d.bmp", filezip_number++);
		fpzip = fopen (filezipname, "r");

		if (fpzip == NULL) {
			printf ("Error: could not open ZIP File\n");
			// filezip_number=0;
			OSMutexPost(MutexMemory);
			OSTaskDel(OS_PRIO_SELF);
		} else {
			printf ("Opened ZIP File %02d\n",filezip_number-1);
			fclose (fpzip);
			int value = OSSemAccept(SemaphoreMemory);
			if(value > 0){

				PixelMem[filezip_number-1][0]=OSMemGet(ImageMemory, &err);

				alt_ucosii_check_return_code(err);

				// query sobre la particion de memoria
				OSMemQuery(ImageMemory, &mem_data);
				printf("Number of memory blocks = %ld, Free = %ld, Used = %ld \n",mem_data.OSNBlks,mem_data.OSNFree,mem_data.OSNUsed);
				printf("Size (in bytes) of each memory block = %lu,\n",mem_data.OSBlkSize);
				printf("Pointer to the beginning address of the memory partition = %lu,\n",mem_data.OSAddr);
				printf("Pointer to the reserved memory block = %lu,\n",&PixelMem[filezip_number-1][0]);
				printf("Pointer to the beginning of the free list of memory blocks = %lu,\n",mem_data.OSFreeList);



				Read_BMP_ZipFile(filezipname);

				// OSMemPut(ImageMemory, PixelMem[filezip_number-1]);
				// OSSemPost(SemaphoreMemory);
				// alt_ucosii_check_return_code(error);
			}else{
				OSMutexPost(MutexMemory);
				OSTaskDel(OS_PRIO_SELF);
			}

		}

		OSMutexPost(MutexMemory);


    OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

//Subrutina que lee un archivo BMP de la Flash
short int Read_BMP_ZipFile(char *file_name)
{
	unsigned char bmp_header[54];
	unsigned char pixel_bytes[2];
	unsigned char row_padding[3];
	unsigned int inicio_imagen;
	unsigned int compresion;
	int anchura_imagen;
	int altura_imagen;
	int altura_absoluta;
	int top_down;
	unsigned short bitsxpixel;
	int bytes_por_fila;
	int padding_por_fila;
	int row, col, row_destino, offset;
	unsigned short int pixel_color;
	FILE *fpzip;

	fpzip = fopen(file_name, "rb");
	if (fpzip == NULL) {
		printf("Error al abrir archivo BMP\n");
		return -1;
	}

	if (fread(bmp_header, 1, sizeof(bmp_header), fpzip) != sizeof(bmp_header)) {
		printf("Error al leer cabecera BMP\n");
		fclose(fpzip);
		return -2;
	}

	if ((bmp_header[0] != 'B') || (bmp_header[1] != 'M')) {
		printf("Formato archivo incorrecto\n");
		fclose(fpzip);
		return -3;
	}

	inicio_imagen = (unsigned int)bmp_header[10] |
						((unsigned int)bmp_header[11] << 8) |
						((unsigned int)bmp_header[12] << 16) |
						((unsigned int)bmp_header[13] << 24);
	anchura_imagen = (int)((unsigned int)bmp_header[18] |
						((unsigned int)bmp_header[19] << 8) |
						((unsigned int)bmp_header[20] << 16) |
						((unsigned int)bmp_header[21] << 24));
	altura_imagen = (int)((unsigned int)bmp_header[22] |
					   ((unsigned int)bmp_header[23] << 8) |
					   ((unsigned int)bmp_header[24] << 16) |
					   ((unsigned int)bmp_header[25] << 24));
	bitsxpixel = (unsigned short)((unsigned short)bmp_header[28] |
						   ((unsigned short)bmp_header[29] << 8));
	compresion = (unsigned int)bmp_header[30] |
					 ((unsigned int)bmp_header[31] << 8) |
					 ((unsigned int)bmp_header[32] << 16) |
					 ((unsigned int)bmp_header[33] << 24);

	if (bitsxpixel != 16) {
		printf("Formato no compatible: se esperaba RGB565 (16 bpp)\n");
		fclose(fpzip);
		return -3;
	}

	if ((compresion != 0) && (compresion != 3)) {
		printf("Compresion BMP no compatible para RGB565\n");
		fclose(fpzip);
		return -3;
	}

	if ((anchura_imagen <= 0) || (altura_imagen == 0)) {
		printf("Dimensiones BMP incorrectas\n");
		fclose(fpzip);
		return -3;
	}

	top_down = (altura_imagen < 0);
	altura_absoluta = top_down ? (-altura_imagen) : altura_imagen;

	printf("Nombre Archivo = %s\n", file_name);
	printf("Anchura Imagen = %d\n", anchura_imagen);
	printf("Altura  Imagen = %d\n", altura_absoluta);
	printf("Bits por pixel = %d\n", bitsxpixel);

	bytes_por_fila = anchura_imagen * 2;
	padding_por_fila = (4 - (bytes_por_fila & 0x3)) & 0x3;

	if (fseek(fpzip, (long)inicio_imagen, SEEK_SET) != 0) {
		printf("Error posicionando inicio de datos BMP\n");
		fclose(fpzip);
		return -2;
	}

	for (row = 0; row < altura_absoluta; row++)
	{
		row_destino = top_down ? row : (altura_absoluta - 1 - row);
		for (col = 0; col < anchura_imagen; col++)
		{
			if (fread(pixel_bytes, 1, 2, fpzip) != 2) {
				printf("Error leyendo pixel BMP\n");
				fclose(fpzip);
				return -2;
			}

			/* RGB565 en little-endian: byte bajo + byte alto */
			pixel_color = (unsigned short int)(((unsigned short int)pixel_bytes[1] << 8) |
										   (unsigned short int)pixel_bytes[0]);

			offset = (row_destino << 9) + col;
			// para 4 bloques
#ifndef UNBLOQUE
			PixelMem[filezip_number-1][offset]=pixel_color;
			//para 1 bloque
#else
			PixelMem[0][offset]=pixel_color;
#endif
		}

		if (padding_por_fila > 0) {
			if (fread(row_padding, 1, (size_t)padding_por_fila, fpzip) != (size_t)padding_por_fila) {
				printf("Error leyendo padding de fila BMP\n");
				fclose(fpzip);
				return -2;
			}
		}
	}

	fclose(fpzip);

	return 0;
}

