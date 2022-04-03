/* ***************************************************************************
 * File: public.h
 * Description: contains public prototypes and shared macros
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author        	Date      		Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                                  - v1
 *****************************************************************************/

#ifndef PUBLIC_H
#define PUBLIC_H

//#define     CONSOLE     C_LCD
#define     CONSOLE     C_UART1
    

void vStartTask1(void);
void vStartTask2(void);

#endif /* PUBLIC_H */