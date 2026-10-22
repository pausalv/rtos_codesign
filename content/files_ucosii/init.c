#include "..\inc\init.h"


//Text on VGA
char texto_up[80] = 	"                        Understanding RTOS on VGA - CHS 2026/27                ";
char texto_down[80] = 	"                      uCOS-II over NIOS-II, Cyclone V, CHS 2026/27             ";
char blank[80]=         "                                                                               \0";

//Global variable to write lines on VGA
int line=3;

// prototype of UCOS Utilities
void Init_UCOS_Utilities(void);
 
// declaration of UCOS utilities
//OS_EVENT * Semaphore;

// Initialization function
void Init_App()
{
    VGA_Clean_Full_Lines(1,60,character_buffer);

//Text on first line
    VGA_text (2, 0, texto_up,character_buffer);		// first line
    VGA_text (2, 1, texto_down, character_buffer);  // second line

//Boxes on VGA
    VGA_box (0, 0, 80*4-1, 60*4-1, 0x1111,pixel_buffer);	 	//Big blue box on VGA
    VGA_box (0, 0, 320-1, 8, 0x01100,pixel_buffer);  		//Black box on top area of VGA
    /* pink box on right area */
    VGA_box (155, 10, 315, 235, 0xF81F,pixel_buffer);	//Pink box on right area
// VGA_box (155, 10, 315, 235, 0x0100,pixel_buffer);	//Black box on right area


// OFF all leds on board
    Led_OFF_All(LED_ptr);   

// Enable interrupts from pushbuttons
//*(KEY_ptr + 2)=0xF; // Enable interrupts for all 4 pushbuttons
//alt_irq_register(PUSHBUTTONS_IRQ, NULL, (alt_isr_func) pushbutton_isr);

    
/* Initialization of UCOS utilities */
    Init_UCOS_Utilities();

}

/*
 * @brief Prints string on MTL at line position "line"
 * @param input char visualiza_string[40] is the string to be visualized on MTL
 * @param output result of the position to visualize the string
 *
 * @warning non-reentrant function as line is a global shared variable
 */
void Print_VGA(char visualiza_string[35], int* line)
{
    int first_line=3;
    int last_line=59;  // VGA text rows are 0..59

    VGA_text (2, *line, visualiza_string, character_buffer);
    VGA_Clean_Lines(*line+1,*line+2,character_buffer);

    *line=*line+1;
    if (*line>last_line){
        *line=first_line;
    }
}

/* UCOS Utilities */
void Init_UCOS_Utilities(void)
{
  //  Semaphore = OSSemCreate(1);
}
