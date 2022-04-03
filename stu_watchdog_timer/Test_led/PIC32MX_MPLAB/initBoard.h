/**
  
*/

#ifndef INITBOARD_H
#define INITBOARD_H

#ifdef	__cplusplus
extern "C" {
#endif
/**
    Section: Includes
*/
#include <stdint.h>
#include <xc.h>
/**
    Section: Macros
*/
#define io_init     initIOs
/**
    Section: Function Prototypes
*/
//void initTimer2( void);
void initIOs(void);

#ifdef	__cplusplus
}
#endif
#endif
