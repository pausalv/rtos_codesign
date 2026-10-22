/*
 * showpicture.c
 *
 *  Created on: 20 de may. de 2024
 *      Author: mpeiro
 */



#include <string.h>
#include "..\inc\showpicture.h"

void ShowPicture(void* pdata)
{
static INT8U i=0;
INT8U err;
	while(1){


	printf("Hello from ShowPicture\n");
OSSchedLock();
// caso 2 bloques de memoria
#ifndef UNBLOQUE
	memmove(pixel_buffer,&PixelMem[i],512*240*2);
// OSSchedUnlock();
	i++;
	if (i==2){
		i=0;
	}

// caso de 1 bloque de memoria
#else
	memmove(pixel_buffer,&PixelMem[0],512*240*2);
	err=OSMemPut(ImageMemory, PixelMem[0]);
	alt_ucosii_check_return_code(err);
	err=OSSemPost(SemaphoreMemory);
	alt_ucosii_check_return_code(err);
	//	alt_ucosii_check_return_code(err);
#endif
OSSchedUnlock();
    OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
