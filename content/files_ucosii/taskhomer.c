/*
 * taskhomer.c
 *
 *  Created on: 13 de may. de 2024
 *      Author: mpeiro
 */


#include "..\inc\taskhomer.h"

/* Prints "HOMER" and sleeps for x seconds */
void TaskHomer(void* pdata)
{
	INT8U err;

	char visualiza_string[40] = "Hello from Homer";
	char blank[30]="                             \0";
	char visualiza_string_homer[15][40]=
	{					            "Why you, little bastard!",
                        "        ___             ",
                        "       //_\\\\_           ",
                        "     .\"      \".         ",
                        "    /          \\        ",
                        "    |           \\_      ",
                        "    |       ,--.-.)     ",
                        "     \\     /  o \\o\\     ",
                        "     /\\/\\  \\    /_/     ",
                        "      (_.   `--'__)     ",
                        "       |     .-'  \\     ",
                        "       |  .-'.     )    ",
                        "       | (  _/--.-'     ",
                        "       |  `.___.'       ",
                        "       |     (          "};


	int homer_pos=10; //start VGA line to draw

  /*
    int *pMailBox_homer = &homer_pos;
  */

  while (1)
  {

	  OSTimeDlyHMSM(0, 0, 7, 0);
    //receive message from mailbox
/*
    pMailBox_homer = (int*) OSMboxPend(SharedMail,1000,&err);
    if(err == OS_ERR_NONE){
      homer_pos = *pMailBox_homer;
      OSSemPend(Semaphore,0,&err);
      alt_ucosii_check_return_code(err);
      printf("Mail is %d: new start position is %d\n",*pMailBox_homer,homer_pos);
      err = OSSemPost(Semaphore);
      alt_ucosii_check_return_code(err);
    }
*/

    // Led_ON_All(red_LED_ptr);
    Led_OFF_All(LED_ptr);


    //clean text on black box
    for (int var = 3; var < 60; ++var) {
      VGA_text (40, var, blank,character_buffer);
    }
    //creates picture
    for (int var = 0; var < 15; ++var) {
      VGA_text (50, homer_pos+var, visualiza_string_homer[var],character_buffer);
    }
	  
    printf("%s\n",visualiza_string);

	}
}


