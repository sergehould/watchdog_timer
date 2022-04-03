/**
   	FileName:     util.c
	
 
	Description: 
      
 * * REVISION HISTORY:
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author        	Date            Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Serge Hould      29 Sept. 2021	v1.0
 * Serge Hould      13 Dec 2021     v1.1 Adapt for MICROSTICK II
 * 
 * TO DO: fine tune delay for MICROSTICK II 
 * 		
 *******************************************************************************/
#include <xc.h>
#include "util.h"

/*********************** Heartbeat section ************************************
 *
 * Description: Blinks a LED when repeatedly called from the main loop.
 *              The frequency decreases when the CPU is loaded.
 * 
 * REVISION HISTORY:
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author        	Date            Version     Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Serge Hould      15 Sept. 2021   1.0         First version 
 * 
 * 		
 *******************************************************************************/

#define     SKIP        20000    // Sets the heartbeat frequency

//#define     DUTY        50     // 0.1 % duty cycle
#define     DUTY      500     // 1 % duty cycle
//#define     DUTY      5000     // 10 % duty cycle
//#define     DUTY       50000   // 100% duty cycle

#if defined  MX3
    //#define     HEARTBEAT    LATDbits.LATD12  //Green LED
    #define     HEARTBEAT    LATDbits.LATD3  // Blue LED
    //#define     HEARTBEAT    LATDbits.LATD2  // Red LED
#elif   defined EXPLORER_16_32
    #define     HEARTBEAT    LATAbits.LATA7  // D10 LED

#elif defined MICROSTICK_II
    #define     HEARTBEAT      LATAbits.LATA0
#endif


void heartbeat(void){
    static int cnt =0;
    if(cnt++ > SKIP){
        cnt = 0;
        HEARTBEAT = 1;
    }
    else if (cnt == DUTY )HEARTBEAT = 0;
}
 
#if defined  MX3
void init_heartbeat(void){
        // Configure RGBLEDs as digital outputs.

    //    RPD2R = 0x0B; // LED8_R RPD2 is OC3 - for PWM usage
    RPD2R = 0;      // no remapable
    TRISDbits.TRISD2 = 0;    // RED output 
  
    //RPD12R 1011 = OC5
    //   RPD12R = 0x0B; // LED8_G RPD12 is OC5 - for PWM usage
    RPD12R = 0;      // no remapable
    TRISDbits.TRISD12 = 0;    // Green  output
 
    //    RPD3R = 0x0B; // LED8_B RPD3 is OC4 - for PWM usage
    RPD3R = 0;      // no remapable
    TRISDbits.TRISD3 = 0;    // Blue output
    
    // disable analog (set pins as digital))
    ANSELDbits.ANSD2 = 0;
    ANSELDbits.ANSD3= 0;
    
    // Turn them off
    LATDbits.LATD2 = 0;     // Red
    LATDbits.LATD12 = 0;    // Green
    LATDbits.LATD3= 0;      // Blue
}
#elif   defined EXPLORER_16_32
void init_heartbeat(void){
      TRISAbits.TRISA7 = 0;       //LED D10
}

#elif defined MICROSTICK_II
void init_heartbeat(void){
      TRISAbits.TRISA0  = 0;       //LED D6
}
#endif

/******************** End of Heartbeat section*********************************/

/*************************** Tone section ************************************
 * 
	Description: 
      
 * * REVISION HISTORY:
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author        	Date            Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Serge Hould      22 Sept. 2021	v1.0
 * 
 * 
 * 		
 *******************************************************************************/
#if defined  MX3
#define BOARD_FRQ       80000000
#define FREQ_SINE       48000

// This array contains the values that implement one syne period, over 25 samples. 
// They are generated using this site: 
// http://www.daycounter.com/Calculators/Sine-Generator-Calculator.phtml
static unsigned short sinSamples[] = {
256,320,379,431,472,499,511,507,488,453,
406,350,288,224,162,106, 59, 24,  5,  1,
 13, 40, 81,133,192
};

/* Generate a high pitch tone for the number of mS passed by the argument cnt */
void tone_high(int cnt){
    int i;
    cnt = cnt*38;
    while(cnt-- > 0){
        OC1RS = 4*sinSamples[(++i) % 25];
        delay_10us(2);  // 20uS
    }
}
/* Generate a low pitch tone for the number of mS passed by the argument cnt */
void tone_low(int cnt){
    int i;
    cnt = cnt * 20;
    while(cnt-- > 0){
        OC1RS = 4*sinSamples[(++i) % 25];
        delay_10us(4);  // 20uS
    }
}

void init_tone(void){
        // Configure AUDIO output as digital output.
    TRISBbits.TRISB14 = 0;    
    RPB14R = 0x0C; // 1100 = OC1
    // disable analog (set pins as digital)
    ANSELBbits.ANSB14 = 0;
    
    PR3 = (int)((float)((float)BOARD_FRQ/FREQ_SINE) + 0.5);               
    TMR3 = 0;
    T3CONbits.TCKPS = 0;     //1:1 prescale value
    T3CONbits.TGATE = 0;     //not gated input (the default)
    T3CONbits.TCS = 0;       //PCBLK input (the default)
    T3CONbits.ON = 1;        //turn on Timer3
 
    OC1CONbits.ON = 0;       // Turn off OC1 while doing setup.
    OC1CONbits.OCM = 6;      // PWM mode on OC1; Fault pin is disabled
    OC1CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
    OC1CONbits.ON = 1;       // Start the OC1 module      
}
#endif
/************************ End of Tone section*********************************/

#if (defined  MX3) || (defined EXPLORER_16_32)
/*********************** Blocking Delay section *******************************
**    void delay_10us( unsigned int  t10usDelay )
**
**	Synopsis:
**		delay_10us(100)  // 100*10uS = 1mS
**
**	Parameters:
**		t10usDelay - the amount of time you wish to delay in tens of microseconds
**
**	Return Values:
**      none
**
**	Errors:
**		none
**
**	Description:
**		This procedure delays program execution for the specified number
**      of microseconds. This delay is not precise.
**		
**	Note:
**		This routine is written with the assumption that the
**		system clock is 80 MHz.
*/
void delay_10us( unsigned int  t10usDelay )
{
    int j;
    while ( 0 < t10usDelay )
    {
        t10usDelay--;
        j = 14;
        while ( 0 < j )
        {
            j--;
        }   // end while 
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
         
    }   // end while
}


/******************************************************************************
**    void delay_ms( unsigned int  tmsDelay )
**
**	Synopsis:
**		delay_ms(100)  // 100*1mS = 100mS
**
**	Parameters:
**		tmsDelay - the amount of time you wish to delay in milliseconds
**
**	Return Values:
**      none
**
**	Errors:
**		none
**
**	Description:
**		This procedure delays program execution for the specified number
**      of milliseconds. This delay is not precise.
**		
**	Note:
**		This routine is written with the assumption that the
**		system clock is 80 MHz.
*/
void delay_ms( unsigned int  tmsDelay )
{
    int j;
    tmsDelay *=100;
    while ( 0 < tmsDelay )
    {
        tmsDelay--;
        j = 14;
        while ( 0 < j )
        {
            j--;
        }   // end while 
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
         
    }   // end while
}

#elif defined MICROSTICK_II
/*********************** Blocking Delay section *******************************
**    void delay_10us( unsigned int  t10usDelay )
**
**	Synopsis:
**		delay_10us(100)  // 100*10uS = 1mS
**
**	Parameters:
**		t10usDelay - the amount of time you wish to delay in tens of microseconds
**
**	Return Values:
**      none
**
**	Errors:
**		none
**
**	Description:
**		This procedure delays program execution for the specified number
**      of microseconds. This delay is not precise.
**		
**	Note:
**		This routine is written with the assumption that the
**		system clock is 80 MHz.
*/
void delay_10us( unsigned int  t10usDelay )
{
    int j;
    while ( 0 < t10usDelay )
    {
        t10usDelay--;
        j = 14;
        while ( 0 < j )
        {
            j--;
        }   // end while 
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
         
    }   // end while
}


/******************************************************************************
**    void delay_ms( unsigned int  tmsDelay )
**
**	Synopsis:
**		delay_ms(100)  // 100*1mS = 100mS
**
**	Parameters:
**		tmsDelay - the amount of time you wish to delay in milliseconds
**
**	Return Values:
**      none
**
**	Errors:
**		none
**
**	Description:
**		This procedure delays program execution for the specified number
**      of milliseconds. This delay is not precise.
**		
**	Note:
**		This routine is written with the assumption that the
**		system clock is 80 MHz.
*/
void delay_ms( unsigned int  tmsDelay )
{
    int j;
    tmsDelay *=100;
    while ( 0 < tmsDelay )
    {
        tmsDelay--;
        j = 14;
        while ( 0 < j )
        {
            j--;
        }   // end while 
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
        asm volatile("nop"); // do nothing
         
    }   // end while
}

#endif
/********************* End of Blocking Delay section  section******************/