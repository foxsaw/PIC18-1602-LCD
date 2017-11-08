

#define _XTAL_FREQ 8000000

#include <htc.h>
#include "lcd.h"


// pic18f4520 config bits
__CONFIG(1,INTIO & FCMDIS & IESOEN);
__CONFIG(2,PWRTDIS & BORV42 & BOREN & WDTDIS & WDTPS32K);
__CONFIG(3,PBDIGITAL & CCP2RC1 & LPT1DIS & MCLREN);
__CONFIG(4,XINSTDIS & STVREN & LVPEN & DEBUGDIS);
__CONFIG(5,UNPROTECT);
__CONFIG(6,UNPROTECT);
__CONFIG(7,UNPROTECT);



typedef signed char int8_t; //stdint
typedef unsigned char uint8_t;
typedef signed int int16_t;
typedef unsigned int uint16_t;



//---------------------------------------------------------------------------------------
void delay_ms(uint16_t val)  //delay milliseconds
    {
    while(val--) __delay_ms(1);
    }

//---------------------------------------------------------------------------------------
void main(void)
    {
    IRCF2=1; IRCF1=1; IRCF0=1; //8MHz internal RC oscillator  //Fosc
    SWDTEN=0; //disable watchdog timer
    RBPU=1; //disable pull-up
    ADCON1 = 0b1111;

    lcd_init();
    lcd_print("ABCDEFGHIJKLMNOP");
    lcd_goto(1,0);
    lcd_print("1234567890@#$%^&");

    for(;;)
        {

        }
    }