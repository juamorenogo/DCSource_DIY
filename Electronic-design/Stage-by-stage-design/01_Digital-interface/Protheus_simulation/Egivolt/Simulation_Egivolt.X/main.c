#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define _XTAL_FREQ 1000000UL
#define I2C_SPEED 100000UL
#define I2C1_SPEED 100000UL

// =====================================================
// CONFIG BITS - PIC18LF25K22
// =====================================================
#pragma config FOSC = INTIO67
#pragma config PLLCFG = OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN = OFF
#pragma config IESO = OFF

#pragma config PWRTEN = OFF
#pragma config BOREN = SBORDIS
#pragma config BORV = 190

#pragma config WDTEN = OFF
#pragma config WDTPS = 32768

#pragma config PBADEN = OFF
#pragma config MCLRE = EXTMCLR

#pragma config LVP = OFF
#pragma config XINST = OFF

// =====================================================
// LIBRERIAS
// =====================================================
#include "lcd_i2c.h"
#include "teclado_3x4_pic18lf25k22.h"
#include "mcp4725_i2c2.h"

// =====================================================
// DEFINICIONES
// =====================================================

#define LED LATCbits.LATC0

// Botones
#define BTN_VOLTAJE     PORTBbits.RB0
#define BTN_CORRIENTE   PORTBbits.RB7

#define BOTON_ACTIVO_ALTO 1

#define NUM_BUFFER_SIZE 16

// Normalizacion de setpoints hacia DAC
#define VOLTAJE_MAXIMO   30.0f
#define CORRIENTE_MAXIMA 3.0f

// Direcciones de los DAC
#define DAC_VOLTAJE_ADDR    MCP4725_ADDR_A0_GND
#define DAC_CORRIENTE_ADDR  MCP4725_ADDR_A0_VDD

// =====================================================
// ADC - MEDICION DE VOLTAJE Y CORRIENTE
// =====================================================
//
// Suposicion:
// AN1 / RA1 recibe una senal normalizada de 0 a 5 V
// equivalente a 0 a 30 V medidos.
//
// AN2 / RA2 recibe una senal normalizada de 0 a 5 V
// equivalente a 0 a 3 A medidos.
//
// Las mediciones se guardan en x100 para poder imprimir
// dos decimales sin depender de printf con floats.
// =====================================================

#define ADC_VREF_MV             5000UL

#define ADC_CH_VOLTAJE          1U      // RA1 / AN1
#define ADC_CH_CORRIENTE        2U      // RA2 / AN2

#define VOLTAJE_MED_MAX_X100    3000UL  // 30.00 V
#define CORRIENTE_MED_MAX_X100  300UL   // 3.00 A

// Tiempo entre refrescos de mediciones en reposo
#define LCD_REFRESH_MS          250U

// =====================================================
// TIPOS DE DATOS
// =====================================================

typedef enum
{
    VAR_NINGUNA = 0,
    VAR_VOLTAJE,
    VAR_CORRIENTE
} VariableSeleccionada_t;

// =====================================================
// VARIABLES GLOBALES
// =====================================================

volatile uint8_t boton_voltaje_evento = 0;
volatile uint8_t boton_corriente_evento = 0;
volatile uint8_t entrada_habilitada = 0;

volatile uint8_t rb7_anterior = 0;

VariableSeleccionada_t variable_actual = VAR_NINGUNA;

char numero_buffer[NUM_BUFFER_SIZE];
uint8_t numero_len = 0;
uint8_t numero_tiene_punto = 0;

float voltaje = 0.0f;
float corriente = 0.0f;

uint16_t dac_voltaje_code = 0;
uint16_t dac_corriente_code = 0;

bool dac_voltaje_ok = false;
bool dac_corriente_ok = false;

char voltaje_texto[NUM_BUFFER_SIZE] = "0";
char corriente_texto[NUM_BUFFER_SIZE] = "0";

// Variables de medicion ADC
uint16_t adc_voltaje_raw = 0;
uint16_t adc_corriente_raw = 0;

uint16_t voltaje_adc_mv = 0;
uint16_t corriente_adc_mv = 0;

uint16_t voltaje_medido_x100 = 0;
uint16_t corriente_medida_x100 = 0;

// =====================================================
// PROTOTIPOS
// =====================================================

void MCU_Init(void);

void Botones_Init(void);
void Botones_Actualizar(void);
uint8_t Boton_EstaPresionado(uint8_t estado);

void Entrada_Iniciar(VariableSeleccionada_t variable);
void Entrada_ProcesarTecla(char tecla);
void Entrada_Guardar(void);

void DAC_Init(void);
uint16_t DAC_NormalizarSetpoint(float valor, float valor_maximo);
void DAC_ActualizarVoltaje(float valor);
void DAC_ActualizarCorriente(float valor);

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
void Mediciones_Actualizar(void);

void Numero_ResetBuffer(void);
float Numero_ConvertirAFloat(const char *texto);
void Texto_Copiar(char *destino, const char *origen);

void LCD_MostrarReposo(void);
void LCD_MostrarEntrada(void);
void LCD_MostrarGuardado(void);
void LCD_MostrarMensajeTemporal(const char *mensaje);
void LCD_ActualizarMediciones(void);
void LCD_PrintUInt(uint16_t value);
void LCD_PrintFixed2(uint16_t value_x100);

// =====================================================
// ISR PRINCIPAL
// =====================================================

void __interrupt() ISR(void)
{
    uint8_t lectura;
    uint8_t rb7_actual;

    // -------------------------------------------------
    // Boton de voltaje en RB0 / INT0
    // -------------------------------------------------
    if (INTCONbits.INT0IF == 1)
    {
        INTCONbits.INT0IF = 0;

        if (entrada_habilitada == 0)
        {
            boton_voltaje_evento = 1;
        }
    }

    // -------------------------------------------------
    // RB4-RB7 comparten bandera RBIF.
    // RB4-RB6: columnas del teclado
    // RB7: boton de corriente
    // -------------------------------------------------
    if (INTCONbits.RBIF == 1)
    {
        lectura = PORTB;

        rb7_actual = (lectura & 0b10000000) ? 1 : 0;

#if BOTON_ACTIVO_ALTO
        if ((rb7_actual == 1) && (rb7_anterior == 0))
#else
        if ((rb7_actual == 0) && (rb7_anterior == 1))
#endif
        {
            if (entrada_habilitada == 0)
            {
                boton_corriente_evento = 1;
            }
        }

        rb7_anterior = rb7_actual;
    }

    Teclado_ISR();
}

// =====================================================
// MAIN
// =====================================================

void main(void)
{
    char tecla;

    MCU_Init();

    LCD_Init();

    // Inicializacion de los dos MCP4725 en I2C1 RC3/RC4
    DAC_Init();

    // Inicializa teclado, pero lo deja deshabilitado hasta seleccionar variable.
    Teclado_Init();
    Teclado_Deshabilitar();

    Botones_Init();

    // Inicializa ADC despues de MCU_Init(), porque MCU_Init()
    // deja todos los pines como digitales.
    ADC_Init();

    Numero_ResetBuffer();

    Mediciones_Actualizar();
    LCD_MostrarReposo();

    while (1)
    {
        if (entrada_habilitada == 0)
        {
            // En reposo se atienden los botones y se refrescan mediciones.
            Botones_Actualizar();

            Mediciones_Actualizar();
            LCD_ActualizarMediciones();

            __delay_ms(LCD_REFRESH_MS);
        }
        else
        {
            // En modo edicion solo se atiende el teclado.
            Teclado_Actualizar();

            tecla = Teclado_LeerEvento();

            if (tecla != '\0')
            {
                Entrada_ProcesarTecla(tecla);

                LED = 1;
                __delay_ms(60);
                LED = 0;
            }
        }
    }
}

// =====================================================
// CONFIGURACION GENERAL DEL MICROCONTROLADOR
// =====================================================

void MCU_Init(void)
{
    // Oscilador interno a 1 MHz
    OSCCONbits.IRCF = 0b011;
    OSCCONbits.SCS = 0b10;

    // Todos los pines digitales.
    // ADC_Init() volvera analogicos RA1/AN1 y RA2/AN2.
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;

    // Comparadores apagados
    CM1CON0 = 0x00;
    CM2CON0 = 0x00;

    // LED RC0 como salida
    TRISCbits.TRISC0 = 0;
    LED = 0;
}

// =====================================================
// DAC MCP4725
// =====================================================

void DAC_Init(void)
{
    // Inicializa I2C1 en RC3=SCL1 y RC4=SDA1
    MCP4725_I2C1_Init();

    __delay_ms(50);

    // Verifica ACK de ambos DAC
    dac_voltaje_ok = MCP4725_CheckAddress(DAC_VOLTAJE_ADDR);
    dac_corriente_ok = MCP4725_CheckAddress(DAC_CORRIENTE_ADDR);

    // Inicializa ambos DAC en cero
    DAC_ActualizarVoltaje(0.0f);
    DAC_ActualizarCorriente(0.0f);
}

uint16_t DAC_NormalizarSetpoint(float valor, float valor_maximo)
{
    float resultado;

    if (valor <= 0.0f)
    {
        return 0;
    }

    if (valor >= valor_maximo)
    {
        return 4095;
    }

    resultado = (valor * 4095.0f) / valor_maximo;

    return (uint16_t)(resultado + 0.5f);
}

void DAC_ActualizarVoltaje(float valor)
{
    dac_voltaje_code = DAC_NormalizarSetpoint(valor, VOLTAJE_MAXIMO);
    dac_voltaje_ok = MCP4725_WriteRaw_Fast(DAC_VOLTAJE_ADDR, dac_voltaje_code);
}

void DAC_ActualizarCorriente(float valor)
{
    dac_corriente_code = DAC_NormalizarSetpoint(valor, CORRIENTE_MAXIMA);
    dac_corriente_ok = MCP4725_WriteRaw_Fast(DAC_CORRIENTE_ADDR, dac_corriente_code);
}

// =====================================================
// ADC
// =====================================================

void ADC_Init(void)
{
    /*
     * RA1 / AN1 -> medicion de voltaje normalizada 0-5 V
     * RA2 / AN2 -> medicion indirecta de corriente normalizada 0-5 V
     */

    // RA1 y RA2 como entradas
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;

    // Habilitar funcion analogica en AN1 y AN2
    ANSELAbits.ANSA1 = 1;
    ANSELAbits.ANSA2 = 1;

    /*
     * Referencias del ADC:
     * Vref+ = VDD
     * Vref- = VSS
     */
    ADCON1 = 0x00;

    /*
     * ADCON2:
     * ADFM = 1   -> resultado justificado a la derecha
     * ACQT = 101 -> tiempo de adquisicion 12 TAD
     * ADCS = 001 -> reloj ADC Fosc/8
     *
     * Para Fosc = 1 MHz, Fosc/8 entrega un TAD amplio y estable
     * para esta prueba.
     */
    ADCON2 = 0xA9;

    // Encender ADC
    ADCON0 = 0x01;

    __delay_ms(5);
}

uint16_t ADC_Read(uint8_t channel)
{
    uint16_t result;

    /*
     * Seleccionar canal ADC.
     * En los PIC18, CHS se ubica normalmente desde el bit 2.
     * ADON se mantiene en 1.
     */
    ADCON0 = (uint8_t)((channel << 2) | 0x01);

    // Tiempo de adquisicion
    __delay_us(30);

    // Iniciar conversion: GO/DONE = 1
    ADCON0bits.GO = 1;

    // Esperar fin de conversion
    while (ADCON0bits.GO)
    {
        ;
    }

    result = ((uint16_t)ADRESH << 8) | ADRESL;

    return result;
}

// =====================================================
// MEDICIONES
// =====================================================

void Mediciones_Actualizar(void)
{
    adc_voltaje_raw = ADC_Read(ADC_CH_VOLTAJE);
    adc_corriente_raw = ADC_Read(ADC_CH_CORRIENTE);

    /*
     * Voltaje real en pines ADC:
     * 0 a 1023 -> 0 a 5000 mV
     */
    voltaje_adc_mv =
        (uint16_t)((((uint32_t)adc_voltaje_raw * ADC_VREF_MV) + 511UL) / 1023UL);

    corriente_adc_mv =
        (uint16_t)((((uint32_t)adc_corriente_raw * ADC_VREF_MV) + 511UL) / 1023UL);

    /*
     * Escalamiento de medicion:
     *
     * AN1: 0 a 5 V -> 0 a 30 V
     * AN2: 0 a 5 V -> 0 a 3 A
     *
     * Se guarda en x100:
     * 30.00 V -> 3000
     * 3.00 A  -> 300
     */
    voltaje_medido_x100 =
        (uint16_t)((((uint32_t)adc_voltaje_raw * VOLTAJE_MED_MAX_X100) + 511UL) / 1023UL);

    corriente_medida_x100 =
        (uint16_t)((((uint32_t)adc_corriente_raw * CORRIENTE_MED_MAX_X100) + 511UL) / 1023UL);
}

// =====================================================
// CONFIGURACION DE BOTONES
// =====================================================

void Botones_Init(void)
{
    uint8_t lectura;

    // RB0 / INT0: boton de voltaje
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB0 = 0;

#if BOTON_ACTIVO_ALTO
    INTCON2bits.INTEDG0 = 1;
#else
    INTCON2bits.INTEDG0 = 0;
#endif

    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;

    // RB7 / IOC: boton de corriente
    TRISBbits.TRISB7 = 1;

    IOCB |= 0b10000000;

    lectura = PORTB;
    rb7_anterior = (lectura & 0b10000000) ? 1 : 0;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;

    INTCONbits.GIE = 1;
}

uint8_t Boton_EstaPresionado(uint8_t estado)
{
#if BOTON_ACTIVO_ALTO
    return (estado != 0);
#else
    return (estado == 0);
#endif
}

void Botones_Actualizar(void)
{
    if (boton_voltaje_evento == 1)
    {
        boton_voltaje_evento = 0;

        __delay_ms(25);

        if (Boton_EstaPresionado(BTN_VOLTAJE))
        {
            Entrada_Iniciar(VAR_VOLTAJE);
            return;
        }
    }

    if (boton_corriente_evento == 1)
    {
        boton_corriente_evento = 0;

        __delay_ms(25);

        if (Boton_EstaPresionado(BTN_CORRIENTE))
        {
            Entrada_Iniciar(VAR_CORRIENTE);
            return;
        }
    }
}

// =====================================================
// CONTROL DE ENTRADA NUMERICA
// =====================================================

void Entrada_Iniciar(VariableSeleccionada_t variable)
{
    variable_actual = variable;
    entrada_habilitada = 1;

    boton_voltaje_evento = 0;
    boton_corriente_evento = 0;

    Numero_ResetBuffer();

    Teclado_Interrupcion_Init();

    LCD_MostrarEntrada();
}

void Entrada_ProcesarTecla(char tecla)
{
    if (tecla >= '0' && tecla <= '9')
    {
        if (numero_len < (NUM_BUFFER_SIZE - 1))
        {
            numero_buffer[numero_len] = tecla;
            numero_len++;
            numero_buffer[numero_len] = '\0';

            LCD_MostrarEntrada();
        }
        else
        {
            LCD_MostrarMensajeTemporal("Buffer lleno");
        }

        return;
    }

    if (tecla == '#')
    {
        if (numero_tiene_punto)
        {
            LCD_MostrarMensajeTemporal("Ya tiene punto");
            return;
        }

        if (numero_len < (NUM_BUFFER_SIZE - 1))
        {
            if (numero_len == 0)
            {
                numero_buffer[numero_len] = '0';
                numero_len++;
            }

            if (numero_len < (NUM_BUFFER_SIZE - 1))
            {
                numero_buffer[numero_len] = '.';
                numero_len++;
                numero_buffer[numero_len] = '\0';

                numero_tiene_punto = 1;

                LCD_MostrarEntrada();
            }
        }
        else
        {
            LCD_MostrarMensajeTemporal("Buffer lleno");
        }

        return;
    }

    if (tecla == '*')
    {
        Entrada_Guardar();
        return;
    }
}

void Entrada_Guardar(void)
{
    float valor_ingresado;

    if (numero_len == 0)
    {
        LCD_MostrarMensajeTemporal("Ingrese un valor");
        return;
    }

    valor_ingresado = Numero_ConvertirAFloat(numero_buffer);

    if (variable_actual == VAR_VOLTAJE)
    {
        if (valor_ingresado > VOLTAJE_MAXIMO)
        {
            LCD_MostrarMensajeTemporal("Maximo 30 V");
            return;
        }

        Texto_Copiar(voltaje_texto, numero_buffer);
        voltaje = valor_ingresado;

        DAC_ActualizarVoltaje(voltaje);
    }
    else if (variable_actual == VAR_CORRIENTE)
    {
        if (valor_ingresado > CORRIENTE_MAXIMA)
        {
            LCD_MostrarMensajeTemporal("Maximo 3 A");
            return;
        }

        Texto_Copiar(corriente_texto, numero_buffer);
        corriente = valor_ingresado;

        DAC_ActualizarCorriente(corriente);
    }

    LCD_MostrarGuardado();

    __delay_ms(1000);

    entrada_habilitada = 0;
    variable_actual = VAR_NINGUNA;

    boton_voltaje_evento = 0;
    boton_corriente_evento = 0;

    Numero_ResetBuffer();

    Teclado_Deshabilitar();

    Mediciones_Actualizar();
    LCD_MostrarReposo();
}

// =====================================================
// BUFFER NUMERICO
// =====================================================

void Numero_ResetBuffer(void)
{
    for (uint8_t i = 0; i < NUM_BUFFER_SIZE; i++)
    {
        numero_buffer[i] = '\0';
    }

    numero_len = 0;
    numero_tiene_punto = 0;
}

float Numero_ConvertirAFloat(const char *texto)
{
    float valor = 0.0f;
    float factor_decimal = 0.1f;
    uint8_t despues_punto = 0;

    while (*texto != '\0')
    {
        if (*texto == '.')
        {
            despues_punto = 1;
        }
        else if (*texto >= '0' && *texto <= '9')
        {
            if (despues_punto == 0)
            {
                valor = (valor * 10.0f) + (float)(*texto - '0');
            }
            else
            {
                valor = valor + ((float)(*texto - '0') * factor_decimal);
                factor_decimal = factor_decimal * 0.1f;
            }
        }

        texto++;
    }

    return valor;
}

void Texto_Copiar(char *destino, const char *origen)
{
    uint8_t i;

    for (i = 0; i < (NUM_BUFFER_SIZE - 1); i++)
    {
        destino[i] = origen[i];

        if (origen[i] == '\0')
        {
            return;
        }
    }

    destino[NUM_BUFFER_SIZE - 1] = '\0';
}

// =====================================================
// LCD
// =====================================================

void LCD_MostrarReposo(void)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set V:");
    LCD_Print(voltaje_texto);
    LCD_Print(" I:");
    LCD_Print(corriente_texto);

    LCD_SetCursor(1, 0);
    LCD_Print("RB0=V  RB7=I");

    LCD_ActualizarMediciones();
}

void LCD_ActualizarMediciones(void)
{
    LCD_ClearLine(2);
    LCD_SetCursor(2, 0);
    LCD_Print("Vmed: ");
    LCD_PrintFixed2(voltaje_medido_x100);
    LCD_Print(" V");

    LCD_ClearLine(3);
    LCD_SetCursor(3, 0);
    LCD_Print("Imed: ");
    LCD_PrintFixed2(corriente_medida_x100);
    LCD_Print(" A");
}

void LCD_MostrarEntrada(void)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);

    if (variable_actual == VAR_VOLTAJE)
    {
        LCD_Print("Editando: Voltaje");
    }
    else if (variable_actual == VAR_CORRIENTE)
    {
        LCD_Print("Editando: Corriente");
    }

    LCD_SetCursor(1, 0);
    LCD_Print("Valor: ");

    if (numero_len > 0)
    {
        LCD_Print(numero_buffer);
    }
    else
    {
        LCD_Print("-");
    }

    LCD_SetCursor(2, 0);
    LCD_Print("# = punto decimal");

    LCD_SetCursor(3, 0);
    LCD_Print("* = guardar");
}

void LCD_MostrarGuardado(void)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Valor guardado");

    if (variable_actual == VAR_VOLTAJE)
    {
        LCD_SetCursor(1, 0);
        LCD_Print("Voltaje: ");
        LCD_Print(voltaje_texto);
        LCD_Print(" V");

        LCD_SetCursor(2, 0);
        LCD_Print("DAC V: ");

        if (dac_voltaje_ok)
        {
            LCD_Print("OK");
        }
        else
        {
            LCD_Print("ERROR");
        }

        LCD_SetCursor(3, 0);
        LCD_Print("Vmed: ");
        LCD_PrintFixed2(voltaje_medido_x100);
        LCD_Print(" V");
    }
    else if (variable_actual == VAR_CORRIENTE)
    {
        LCD_SetCursor(1, 0);
        LCD_Print("Corriente: ");
        LCD_Print(corriente_texto);
        LCD_Print(" A");

        LCD_SetCursor(2, 0);
        LCD_Print("DAC I: ");

        if (dac_corriente_ok)
        {
            LCD_Print("OK");
        }
        else
        {
            LCD_Print("ERROR");
        }

        LCD_SetCursor(3, 0);
        LCD_Print("Imed: ");
        LCD_PrintFixed2(corriente_medida_x100);
        LCD_Print(" A");
    }
}

void LCD_MostrarMensajeTemporal(const char *mensaje)
{
    LCD_ClearLine(3);
    LCD_SetCursor(3, 0);
    LCD_Print(mensaje);

    __delay_ms(700);

    LCD_MostrarEntrada();
}

// =====================================================
// FUNCIONES AUXILIARES LCD
// =====================================================

void LCD_PrintUInt(uint16_t value)
{
    char buffer[6];
    uint8_t i = 0;
    uint8_t j;

    if (value == 0)
    {
        LCD_Char('0');
        return;
    }

    while ((value > 0) && (i < sizeof(buffer)))
    {
        buffer[i] = (char)('0' + (value % 10));
        value /= 10;
        i++;
    }

    for (j = i; j > 0; j--)
    {
        LCD_Char(buffer[j - 1]);
    }
}

void LCD_PrintFixed2(uint16_t value_x100)
{
    uint16_t entero;
    uint8_t decimal;

    entero = value_x100 / 100;
    decimal = (uint8_t)(value_x100 % 100);

    LCD_PrintUInt(entero);
    LCD_Char('.');
    LCD_Char((char)('0' + (decimal / 10)));
    LCD_Char((char)('0' + (decimal % 10)));
}
