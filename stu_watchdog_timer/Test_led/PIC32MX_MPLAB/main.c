/***************************************************************************** 
 * Author   Date            Version     Comment
 * SH       April 2022      1.0         Watchdog timer template
 * 
 *     TO COMPLETE BY THE STUDENT
 *****************************************************************************/


/* peripheral library include */
#include <stdio.h>
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" 
#include "semphr.h"
#include "croutine.h"
#include "console.h"
#include "initBoard.h"
#include "public.h"


/* Prototypes for the standard FreeRTOS callback/hook functions implemented within this file. */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );


int main( void ){
    
    /* Prepare the hardware */
	INTCONbits.MVEC=1; // enable multiVectoredInt. 
     
     
    /* Tasks creation */
        
	/* Start the tasks and timer running. */
    vTaskStartScheduler();
	
	

	return 0;
}
/*-----------------------------------------------------------*/

/* vApplicationIdleHook runs only when no task requires the CPU */
void vApplicationIdleHook( void ){

    while(1){
        // your code here
    }
}

/* vApplicationStackOverflowHook */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    pxTask= pxTask;
    pcTaskName = pcTaskName;
		for( ;; );
}