// HD44780 2-line LCD
//
// lcd.h
//
// 16-Oct-2017
//


#include <htc.h>

typedef signed char int8_t; //stdint
typedef unsigned char uint8_t;
typedef signed int int16_t;
typedef unsigned int uint16_t;


//*****************************************************************************

#define RS_PIN   7
#define RS_PORT  PORTC
#define RS_TRIS  TRISC

#define EN_PIN   4
#define EN_PORT  PORTD
#define EN_TRIS  TRISD


#define D4_PIN   6
#define D4_PORT  PORTC
#define D4_TRIS  TRISC

#define D5_PIN   5
#define D5_PORT  PORTC
#define D5_TRIS  TRISC

#define D6_PIN   4
#define D6_PORT  PORTC
#define D6_TRIS  TRISC

#define D7_PIN   3
#define D7_PORT  PORTD
#define D7_TRIS  TRISD
//*****************************************************************************



///////////////////////////////////////////////////////////////////////////////

#ifndef SET_BIT
#define SET_BIT(reg, bit) (reg |= (1<<bit))
#endif

#ifndef CLR_BIT
#define CLR_BIT(reg, bit) (reg &= (~(1<<bit)))
#endif

#ifndef BIT_IS_SET
#define BIT_IS_SET(reg, bit) ((reg>>bit)&1)
#endif

#ifndef BIT_IS_CLR
#define BIT_IS_CLR(reg, bit) (!((reg>>bit)&1))
#endif

///////////////////////////////////////////////////////////////////////////////




#define RS_CLR  CLR_BIT(RS_PORT, RS_PIN)
#define RS_SET  SET_BIT(RS_PORT, RS_PIN)
#define RS_OUT  CLR_BIT(RS_TRIS, RS_PIN)

#define EN_CLR  CLR_BIT(EN_PORT, EN_PIN)
#define EN_SET  SET_BIT(EN_PORT, EN_PIN)
#define EN_OUT  CLR_BIT(EN_TRIS, EN_PIN)


#define D4_CLR  CLR_BIT(D4_PORT, D4_PIN)
#define D4_SET  SET_BIT(D4_PORT, D4_PIN)
#define D4_OUT  CLR_BIT(D4_TRIS, D4_PIN)

#define D5_CLR  CLR_BIT(D5_PORT, D5_PIN)
#define D5_SET  SET_BIT(D5_PORT, D5_PIN)
#define D5_OUT  CLR_BIT(D5_TRIS, D5_PIN)

#define D6_CLR  CLR_BIT(D6_PORT, D6_PIN)
#define D6_SET  SET_BIT(D6_PORT, D6_PIN)
#define D6_OUT  CLR_BIT(D6_TRIS, D6_PIN)

#define D7_CLR  CLR_BIT(D7_PORT, D7_PIN)
#define D7_SET  SET_BIT(D7_PORT, D7_PIN)
#define D7_OUT  CLR_BIT(D7_TRIS, D7_PIN)



#define DDRAM_WRITE  0b10000000
#define CGRAM_WRITE  0b01000000

#define DISPLAY_CLR  0b00000001

#define SCR_LEFT     0b00011000
#define SCR_RIGHT    0b00011100


#define CURSOR_ON    0b00000010
#define CURSOR_OFF   0b00000000

#define BLINK_ON     0b00000001
#define BLINK_OFF    0b00000000



const uint8_t custchar[8*8] =  //64-byte array for 8 custom characters
 {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
  0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
  0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
  0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
  0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,
  0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
  0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
 };


char strbuff[42];



//****************************  prototypes  ***********************************

void lcd_init(void);  //init display

void lcd_bus(uint8_t data);  //write four bit
void lcd_wrt(uint8_t data);  //write 8-bit data

void lcd_dat(uint8_t data);  //write data to the display RAM
void lcd_cmd(uint8_t data);  //write a command
void lcd_cmd5(uint8_t data); //wait 5 ms after write

void lcd_clear(void);  //clear the screen
void lcd_goto(uint8_t line, uint8_t column);  //line 0..1, column 0..39
void lcd_print(const char *str);  //print a string

void lcd_custom(const uint8_t *arr); //loading an array of custom characters

void lcd_right(void);  //shift right
void lcd_left(void);   //shift left

void lcd_cursor_on(void);  //cursor on, blink on
void lcd_cursor_off(void); //cursor off, blink off

//*****************************************************************************



//-----------------------------------------------------------------------------
void lcd_init(void)  //init display
    {
    RS_CLR; RS_OUT;
    EN_CLR; EN_OUT;
    D4_CLR; D4_OUT;
    D5_CLR; D5_OUT;
    D6_CLR; D6_OUT;
    D7_CLR; D7_OUT;

    __delay_ms(10);
    __delay_ms(10); //delay on power up

    lcd_cmd5(0b00000011); //0x03
    lcd_cmd5(0b00000011);
    lcd_cmd5(0b00000011);

    lcd_cmd5(0b00000010); //enable 4-bit mode
    lcd_cmd5(0b00001000); //4-bit mode, 2-line, 5x8 font
    lcd_cmd5(0b00001000); //display off

    lcd_clear();

    lcd_cmd(0b00000110); //entry mode set
    lcd_cmd(0b00001100 | CURSOR_OFF | BLINK_OFF);

    lcd_custom(custchar);

    lcd_goto(0,0);
    }


//-----------------------------------------------------------------------------
void lcd_bus(uint8_t data)  //write four bit
    {
    (data & 0b00000001) ? D4_SET : D4_CLR;
    (data & 0b00000010) ? D5_SET : D5_CLR;
    (data & 0b00000100) ? D6_SET : D6_CLR;
    (data & 0b00001000) ? D7_SET : D7_CLR;

    EN_SET;
    __delay_us(100);
    EN_CLR;
    }


//-----------------------------------------------------------------------------
void lcd_wrt(uint8_t data)  //write data
    {
    lcd_bus(data>>4);
    lcd_bus(data);
    __delay_us(100);
    }


//-----------------------------------------------------------------------------
void lcd_dat(uint8_t data)  //write data to the display RAM
    {
    RS_SET;
    lcd_wrt(data);
    }


//-----------------------------------------------------------------------------
void lcd_cmd(uint8_t data)  //write a command
    {
    RS_CLR;
    lcd_wrt(data);
    }


//-----------------------------------------------------------------------------
void lcd_cmd5(uint8_t data)  //wait 5 ms after write
    {
    lcd_cmd(data);
    __delay_ms(5);
    }


//-----------------------------------------------------------------------------
void lcd_clear(void)  //clear the screen
    {
    lcd_cmd5(DISPLAY_CLR);
    }


//-----------------------------------------------------------------------------
void lcd_goto(uint8_t line, uint8_t column)  //line 0..1, column 0..39
    {
    lcd_cmd(DDRAM_WRITE + ((line & 0x01)<<6) + column);
    }


//-----------------------------------------------------------------------------
void lcd_print(const char *str)  //print a string
    {
    while(*str) lcd_dat(*str++);
    }


//-----------------------------------------------------------------------------
void lcd_custom(const uint8_t *arr)  //loading an array of custom characters
    {
    lcd_cmd(CGRAM_WRITE);
    for(uint8_t k=0; k<64; k++) lcd_dat(arr[k]);
    }


//-----------------------------------------------------------------------------
void lcd_right(void)  //shift right
    {
    lcd_cmd(SCR_RIGHT);
    }


//-----------------------------------------------------------------------------
void lcd_left(void)  //shift left
    {
    lcd_cmd(SCR_LEFT);
    }


//-----------------------------------------------------------------------------
void lcd_cursor_on(void)  //cursor on, blink on
    {
    lcd_cmd(0b00001100 | CURSOR_ON | BLINK_ON);
    }


//-----------------------------------------------------------------------------
void lcd_cursor_off(void)  //cursor off, blink off
    {
    lcd_cmd(0b00001100 | CURSOR_OFF | BLINK_OFF);
    }


