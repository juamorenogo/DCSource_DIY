#ifndef TECLADO_3X4_COLUMNAS_INT_H
#define TECLADO_3X4_COLUMNAS_INT_H

#include <xc.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
    #error "Debe definir _XTAL_FREQ antes de incluir teclado_3x4_columnas_int.h"
#endif

// =====================================================
// NUEVO MAPEO
// =====================================================
//
// Columnas como entradas con interrupcion:
// COL1 -> RB4
// COL2 -> RB5
// COL3 -> RB6
//
// Filas como salidas:
// FILA A -> RC4
// FILA B -> RC5
// FILA C -> RC6
// FILA D -> RC7
//
// Logica:
// - Columnas con pull-down externo.
// - Filas en reposo = 1.
// - Al presionar una tecla, una columna pasa de 0 a 1.
// =====================================================

// Columnas
#define TECLADO_COL_1_IN   PORTBbits.RB4
#define TECLADO_COL_2_IN   PORTBbits.RB5
#define TECLADO_COL_3_IN   PORTBbits.RB6

// Filas
#define TECLADO_FILA_A_OUT LATCbits.LATC2
#define TECLADO_FILA_B_OUT LATCbits.LATC5
#define TECLADO_FILA_C_OUT LATCbits.LATC6
#define TECLADO_FILA_D_OUT LATCbits.LATC7

// =====================================================
// VARIABLES INTERNAS
// =====================================================

static volatile uint8_t teclado_irq_pendiente = 0;
static volatile uint8_t teclado_evento_disponible = 0;
static volatile char teclado_tecla_evento = '\0';

// =====================================================
// PROTOTIPOS
// =====================================================

void Teclado_Init(void);
void Teclado_Interrupcion_Init(void);
void Teclado_ISR(void);
void Teclado_Actualizar(void);
char Teclado_LeerEvento(void);
void Teclado_Deshabilitar(void);

static void Teclado_Filas_Off(void);
static void Teclado_Filas_Idle(void);
static char Teclado_LeerRaw(void);

// =====================================================
// FILAS EN 0
// Se usa durante el barrido.
// =====================================================

static void Teclado_Filas_Off(void)
{
    TECLADO_FILA_A_OUT = 0;
    TECLADO_FILA_B_OUT = 0;
    TECLADO_FILA_C_OUT = 0;
    TECLADO_FILA_D_OUT = 0;
}

// =====================================================
// FILAS EN 1
// Estado de reposo para que una tecla genere interrupcion.
// =====================================================

static void Teclado_Filas_Idle(void)
{
    TECLADO_FILA_A_OUT = 1;
    TECLADO_FILA_B_OUT = 1;
    TECLADO_FILA_C_OUT = 1;
    TECLADO_FILA_D_OUT = 1;
}

// =====================================================
// INICIALIZACION DEL TECLADO
// =====================================================

void Teclado_Init(void)
{
    // RB4-RB6 como entradas
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;

    // RC4-RC7 como salidas
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    // Pines digitales
    ANSELBbits.ANSB4 = 0;
    ANSELBbits.ANSB5 = 0;

    // En algunos headers no existen ANSB6/ANSB7 porque RB6/RB7
    // pueden no ser analogicos. Si tu compilador marca error en
    // estas lineas, simplemente comentalas.
    // ANSELBbits.ANSB6 = 0;

    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC5 = 0;
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;

    // No usar pull-ups internos si estas usando pull-down externos
    INTCON2bits.RBPU = 1;

    Teclado_Filas_Idle();

    __delay_ms(50);
}

// =====================================================
// INICIALIZACION DE INTERRUPCION EN COLUMNAS RB4-RB6
// =====================================================

void Teclado_Interrupcion_Init(void)
{
    uint8_t lectura;

    Teclado_Init();

    // Habilitar interrupt-on-change solo en RB4-RB6.
    // No se toca RB7 para dejarlo disponible al boton.
    IOCB |= 0b01110000;

    lectura = PORTB;
    (void)lectura;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.GIE = 1;
}

// =====================================================
// ISR DEL TECLADO
// Debe llamarse desde la ISR principal.
// =====================================================

void Teclado_ISR(void)
{
    uint8_t lectura;

    if (INTCONbits.RBIF == 1)
    {
        lectura = PORTB;

        // Si el cambio viene de alguna columna del teclado,
        // se marca evento pendiente.
        if ((lectura & 0b01110000) != 0)
        {
            teclado_irq_pendiente = 1;
        }

        INTCONbits.RBIF = 0;
    }
}

// =====================================================
// LECTURA RAW DEL TECLADO
// Retorna '\0' si no hay tecla.
// =====================================================

static char Teclado_LeerRaw(void)
{
    Teclado_Filas_Off();

    // -------------------------
    // Fila A
    // -------------------------
    TECLADO_FILA_A_OUT = 1;
    __delay_us(50);

    if (TECLADO_COL_1_IN == 1) { Teclado_Filas_Idle(); return '1'; }
    if (TECLADO_COL_2_IN == 1) { Teclado_Filas_Idle(); return '2'; }
    if (TECLADO_COL_3_IN == 1) { Teclado_Filas_Idle(); return '3'; }

    TECLADO_FILA_A_OUT = 0;

    // -------------------------
    // Fila B
    // -------------------------
    TECLADO_FILA_B_OUT = 1;
    __delay_us(50);

    if (TECLADO_COL_1_IN == 1) { Teclado_Filas_Idle(); return '4'; }
    if (TECLADO_COL_2_IN == 1) { Teclado_Filas_Idle(); return '5'; }
    if (TECLADO_COL_3_IN == 1) { Teclado_Filas_Idle(); return '6'; }

    TECLADO_FILA_B_OUT = 0;

    // -------------------------
    // Fila C
    // -------------------------
    TECLADO_FILA_C_OUT = 1;
    __delay_us(50);

    if (TECLADO_COL_1_IN == 1) { Teclado_Filas_Idle(); return '7'; }
    if (TECLADO_COL_2_IN == 1) { Teclado_Filas_Idle(); return '8'; }
    if (TECLADO_COL_3_IN == 1) { Teclado_Filas_Idle(); return '9'; }

    TECLADO_FILA_C_OUT = 0;

    // -------------------------
    // Fila D
    // -------------------------
    TECLADO_FILA_D_OUT = 1;
    __delay_us(50);

    if (TECLADO_COL_1_IN == 1) { Teclado_Filas_Idle(); return '*'; }
    if (TECLADO_COL_2_IN == 1) { Teclado_Filas_Idle(); return '0'; }
    if (TECLADO_COL_3_IN == 1) { Teclado_Filas_Idle(); return '#'; }

    Teclado_Filas_Idle();

    return '\0';
}

// =====================================================
// ACTUALIZAR TECLADO
// Se llama frecuentemente en el while(1).
// =====================================================

void Teclado_Actualizar(void)
{
    char tecla;

    if (teclado_irq_pendiente == 0)
    {
        return;
    }

    teclado_irq_pendiente = 0;

    __delay_ms(25);

    tecla = Teclado_LeerRaw();

    if (tecla != '\0')
    {
        teclado_tecla_evento = tecla;
        teclado_evento_disponible = 1;
    }

    Teclado_Filas_Idle();
}

// =====================================================
// LEER EVENTO
// =====================================================

char Teclado_LeerEvento(void)
{
    char tecla;

    if (teclado_evento_disponible == 0)
    {
        return '\0';
    }

    teclado_evento_disponible = 0;
    tecla = teclado_tecla_evento;
    teclado_tecla_evento = '\0';

    return tecla;
}

// =====================================================
// DESHABILITAR TECLADO
// =====================================================

void Teclado_Deshabilitar(void)
{
    uint8_t lectura;

    // Deshabilitar IOC de las columnas RB4-RB6
    // sin afectar RB7.
    IOCB &= 0b10001111;

    lectura = PORTB;
    (void)lectura;

    INTCONbits.RBIF = 0;

    Teclado_Filas_Off();
}

#endif