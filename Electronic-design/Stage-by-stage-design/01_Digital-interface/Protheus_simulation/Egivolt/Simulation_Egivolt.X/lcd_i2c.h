#ifndef LCD_I2C_PCF8574_H
#define LCD_I2C_PCF8574_H

#include <xc.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
    #error "Debe definir _XTAL_FREQ antes de incluir lcd_i2c_pcf8574.h"
#endif

#ifndef I2C_SPEED
    #define I2C_SPEED 100000UL
#endif

// =====================================================
// CONFIGURACION GENERAL LCD I2C
// =====================================================

// PCF8574 con A0=A1=A2=GND
// Direccion 7 bits: 0x20
// Direccion de escritura: 0x20 << 1 = 0x40
#define LCD_I2C_ADDR_W 0x40

// Mapeo PCF8574 -> LCD
#define LCD_RS 0x01    // P0
#define LCD_RW 0x02    // P1
#define LCD_EN 0x04    // P2
#define LCD_BL 0x08    // P3

// Dimensiones de LCD
#define LCD_COLS 20
#define LCD_ROWS 4

// =====================================================
// FUNCIONES I2C2 INTERNAS
// =====================================================

static void LCD_I2C2_Init(void)
{
    // En este proyecto PORTB se configura como digital.
    // RB1 = SCL2
    // RB2 = SDA2
    ANSELB = 0x00;

    TRISBbits.TRISB1 = 1;   // SCL2 como entrada para modulo I2C
    TRISBbits.TRISB2 = 1;   // SDA2 como entrada para modulo I2C

    SSP2CON1 = 0x28;        // I2C Master mode, SSP enabled
    SSP2CON2 = 0x00;
    SSP2STAT = 0x80;        // Slew rate disabled para 100 kHz

    SSP2ADD = ((_XTAL_FREQ / (4UL * I2C_SPEED)) - 1);
}

static void LCD_I2C2_Wait(void)
{
    // Espera a que el modulo MSSP2 termine cualquier operacion I2C
    while((SSP2CON2 & 0x1F) || (SSP2STAT & 0x04))
    {
        // Esperar
    }
}

static void LCD_I2C2_Start(void)
{
    LCD_I2C2_Wait();
    SSP2CON2 |= 0x01;       // SEN = 1
}

static void LCD_I2C2_Stop(void)
{
    LCD_I2C2_Wait();
    SSP2CON2 |= 0x04;       // PEN = 1
}

static void LCD_I2C2_Write(uint8_t data)
{
    LCD_I2C2_Wait();

    SSP2BUF = data;

    // Esperar a que se libere el buffer
    while(SSP2STAT & 0x01)
    {
        // BF = 1 mientras el buffer esta lleno
    }

    LCD_I2C2_Wait();
}

// =====================================================
// FUNCIONES PCF8574 INTERNAS
// =====================================================

static void LCD_PCF8574_Write(uint8_t data)
{
    LCD_I2C2_Start();
    LCD_I2C2_Write(LCD_I2C_ADDR_W);
    LCD_I2C2_Write(data);
    LCD_I2C2_Stop();
}

// =====================================================
// FUNCIONES LCD INTERNAS
// =====================================================

static void LCD_Pulse(uint8_t data)
{
    LCD_PCF8574_Write(data | LCD_EN);
    __delay_us(5);

    LCD_PCF8574_Write(data & ~LCD_EN);
    __delay_us(100);
}

static void LCD_SendNibble(uint8_t nibble, uint8_t rs)
{
    uint8_t data;

    data = (nibble & 0xF0) | LCD_BL;

    if(rs)
    {
        data |= LCD_RS;
    }

    // RW siempre en 0 para escribir en la LCD
    data &= ~LCD_RW;

    LCD_Pulse(data);
}

static void LCD_SendByte(uint8_t value, uint8_t rs)
{
    LCD_SendNibble(value & 0xF0, rs);
    LCD_SendNibble((value << 4) & 0xF0, rs);
}

// =====================================================
// FUNCIONES PUBLICAS LCD
// =====================================================

static void LCD_Cmd(uint8_t cmd)
{
    LCD_SendByte(cmd, 0);
    __delay_ms(2);
}

static void LCD_Char(char character)
{
    LCD_SendByte((uint8_t)character, 1);
}

static void LCD_Clear(void)
{
    LCD_Cmd(0x01);
    __delay_ms(2);
}

static void LCD_Home(void)
{
    LCD_Cmd(0x02);
    __delay_ms(2);
}

static void LCD_SetCursor(uint8_t row, uint8_t col)
{
    static const uint8_t row_offsets[4] = {
        0x00,   // Linea 1
        0x40,   // Linea 2
        0x14,   // Linea 3
        0x54    // Linea 4
    };

    if(row >= LCD_ROWS)
    {
        row = LCD_ROWS - 1;
    }

    if(col >= LCD_COLS)
    {
        col = LCD_COLS - 1;
    }

    LCD_Cmd(0x80 | (row_offsets[row] + col));
}

static void LCD_Print(const char *text)
{
    while(*text != '\0')
    {
        LCD_Char(*text);
        text++;
    }
}

static void LCD_ClearLine(uint8_t row)
{
    LCD_SetCursor(row, 0);

    for(uint8_t i = 0; i < LCD_COLS; i++)
    {
        LCD_Char(' ');
    }

    LCD_SetCursor(row, 0);
}

static void LCD_Backlight_On(void)
{
    LCD_PCF8574_Write(LCD_BL);
}

static void LCD_Backlight_Off(void)
{
    LCD_PCF8574_Write(0x00);
}

static void LCD_Init(void)
{
    LCD_I2C2_Init();

    __delay_ms(50);

    // Secuencia de inicializacion para LCD HD44780 en modo 4 bits
    LCD_SendNibble(0x30, 0);
    __delay_ms(5);

    LCD_SendNibble(0x30, 0);
    __delay_ms(5);

    LCD_SendNibble(0x30, 0);
    __delay_ms(1);

    LCD_SendNibble(0x20, 0);
    __delay_ms(1);

    LCD_Cmd(0x28); // Modo 4 bits, 2 line mode compatible con 20x4
    LCD_Cmd(0x0C); // Display ON, cursor OFF
    LCD_Cmd(0x06); // Incremento automatico
    LCD_Clear();
}

#endif