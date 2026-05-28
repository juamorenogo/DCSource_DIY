#ifndef MCP4725_I2C1_H
#define MCP4725_I2C1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _XTAL_FREQ
    #error "Debe definir _XTAL_FREQ antes de incluir mcp4725_i2c1.h"
#endif

#ifndef I2C1_SPEED
    #define I2C1_SPEED 100000UL
#endif

// =====================================================
// DIRECCIONES MCP4725 - 7 BITS
// =====================================================

#define MCP4725_ADDR_A0_GND  0x60
#define MCP4725_ADDR_A0_VDD  0x61

// =====================================================
// INICIALIZACION I2C1 - RC3/RC4
// =====================================================

static void MCP4725_I2C1_Init(void)
{
    /*
     * I2C1:
     * RC3 = SCL1
     * RC4 = SDA1
     */

    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;

    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    SSP1CON1bits.SSPEN = 0;

    SSP1CON1 = 0x28;       // I2C Master mode, SSP enabled
    SSP1CON2 = 0x00;
    SSP1STAT = 0x80;       // Slew rate disabled para 100 kHz

    SSP1ADD = (uint8_t)((_XTAL_FREQ / (4UL * I2C1_SPEED)) - 1);

    SSP1CON1bits.WCOL = 0;
    SSP1CON1bits.SSPOV = 0;
}

// =====================================================
// FUNCIONES INTERNAS I2C1
// =====================================================

static void MCP4725_I2C1_Wait(void)
{
    while((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04))
    {
        ;
    }
}

static void MCP4725_I2C1_Start(void)
{
    MCP4725_I2C1_Wait();

    SSP1CON2bits.SEN = 1;

    while(SSP1CON2bits.SEN)
    {
        ;
    }
}

static void MCP4725_I2C1_Stop(void)
{
    MCP4725_I2C1_Wait();

    SSP1CON2bits.PEN = 1;

    while(SSP1CON2bits.PEN)
    {
        ;
    }
}

static bool MCP4725_I2C1_Write(uint8_t data)
{
    MCP4725_I2C1_Wait();

    SSP1CON1bits.WCOL = 0;
    SSP1BUF = data;

    while(SSP1STAT & 0x01)
    {
        ;
    }

    MCP4725_I2C1_Wait();

    /*
     * ACKSTAT = 0 -> ACK recibido.
     * ACKSTAT = 1 -> No hubo ACK.
     */
    return (SSP1CON2bits.ACKSTAT == 0);
}

// =====================================================
// FUNCIONES PUBLICAS MCP4725
// =====================================================

static bool MCP4725_CheckAddress(uint8_t address_7bit)
{
    bool ack;

    MCP4725_I2C1_Start();
    ack = MCP4725_I2C1_Write((uint8_t)(address_7bit << 1));
    MCP4725_I2C1_Stop();

    return ack;
}

static bool MCP4725_WriteRaw_Fast(uint8_t address_7bit, uint16_t value_12bit)
{
    bool ok = true;

    if(value_12bit > 4095)
    {
        value_12bit = 4095;
    }

    /*
     * Fast Mode Write:
     *
     * Byte 1:
     * 0000 D11 D10 D9 D8
     *
     * Byte 2:
     * D7 D6 D5 D4 D3 D2 D1 D0
     */
    uint8_t data_high = (uint8_t)((value_12bit >> 8) & 0x0F);
    uint8_t data_low  = (uint8_t)(value_12bit & 0xFF);

    MCP4725_I2C1_Start();

    ok &= MCP4725_I2C1_Write((uint8_t)(address_7bit << 1));
    ok &= MCP4725_I2C1_Write(data_high);
    ok &= MCP4725_I2C1_Write(data_low);

    MCP4725_I2C1_Stop();

    return ok;
}

static bool MCP4725_WriteRaw_Standard(uint8_t address_7bit, uint16_t value_12bit)
{
    bool ok = true;

    if(value_12bit > 4095)
    {
        value_12bit = 4095;
    }

    /*
     * Standard Write DAC Register:
     *
     * Comando 0x40:
     * Escribe el registro DAC, sin escribir EEPROM.
     */
    uint8_t command   = 0x40;
    uint8_t data_high = (uint8_t)(value_12bit >> 4);
    uint8_t data_low  = (uint8_t)((value_12bit & 0x0F) << 4);

    MCP4725_I2C1_Start();

    ok &= MCP4725_I2C1_Write((uint8_t)(address_7bit << 1));
    ok &= MCP4725_I2C1_Write(command);
    ok &= MCP4725_I2C1_Write(data_high);
    ok &= MCP4725_I2C1_Write(data_low);

    MCP4725_I2C1_Stop();

    return ok;
}

static bool MCP4725_WriteRaw_EEPROM(uint8_t address_7bit, uint16_t value_12bit)
{
    bool ok = true;

    if(value_12bit > 4095)
    {
        value_12bit = 4095;
    }

    /*
     * Comando 0x60:
     * Escribe el registro DAC y la EEPROM.
     *
     * No usar dentro de ciclos rápidos en hardware real.
     * Es útil para prueba puntual o simulación.
     */
    uint8_t command   = 0x60;
    uint8_t data_high = (uint8_t)(value_12bit >> 4);
    uint8_t data_low  = (uint8_t)((value_12bit & 0x0F) << 4);

    MCP4725_I2C1_Start();

    ok &= MCP4725_I2C1_Write((uint8_t)(address_7bit << 1));
    ok &= MCP4725_I2C1_Write(command);
    ok &= MCP4725_I2C1_Write(data_high);
    ok &= MCP4725_I2C1_Write(data_low);

    MCP4725_I2C1_Stop();

    __delay_ms(50);

    return ok;
}

static uint16_t MCP4725_VoltageToCode(float voltage, float vref)
{
    if(voltage < 0.0f)
    {
        voltage = 0.0f;
    }

    if(voltage > vref)
    {
        voltage = vref;
    }

    return (uint16_t)((voltage * 4095.0f / vref) + 0.5f);
}

static bool MCP4725_WriteVoltage_Fast(uint8_t address_7bit, float voltage, float vref)
{
    uint16_t code;

    code = MCP4725_VoltageToCode(voltage, vref);

    return MCP4725_WriteRaw_Fast(address_7bit, code);
}

static bool MCP4725_WriteVoltage_Standard(uint8_t address_7bit, float voltage, float vref)
{
    uint16_t code;

    code = MCP4725_VoltageToCode(voltage, vref);

    return MCP4725_WriteRaw_Standard(address_7bit, code);
}

#endif