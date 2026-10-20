/**********************************************************************
 * File:        board.c
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se definen las funciones para el control de las entra-
 *              das y salidas digitales
 *
 * History:
 *   13/10/2026 - original
 **********************************************************************/

#include "board.h"
#include <DSP2833x_Device.h> // Necesario para las macros de los registros
#include <stddef.h>

const int valid_pins_count = 11; // Cantidad de pines digitales validos
const int valid_pins[]     = {
    BOARD_GPIO08_COMMAND_PIN,
    BOARD_GPIO09_COMMAND_PIN,
    BOARD_PRECHARGE_RELAY_COMMAND_PIN,
    BOARD_PRECHARGE_RELAY_STATE_PIN,
    BOARD_QAC1_STATE_PIN,
    BOARD_QDC1_STATE_PIN,
    BOARD_CAC1_STATE_PIN,
    BOARD_CDC1_COMMAND_PIN,
    BOARD_IPM_COMMAND_PIN,
    BOARD_CDC1_STATE_PIN,
    BOARD_CAC1_COMMAND_PIN};

const int command_pins_count = 6; // Cantidad de pines de comando
const int command_pins[]     = {
    BOARD_GPIO08_COMMAND_PIN,
    BOARD_GPIO09_COMMAND_PIN,
    BOARD_PRECHARGE_RELAY_COMMAND_PIN,
    BOARD_CDC1_COMMAND_PIN,
    BOARD_IPM_COMMAND_PIN,
    BOARD_CAC1_COMMAND_PIN};

const int state_pins_count = 5; // Cantidad de pines de estado
const int state_pins[]     = {
    BOARD_PRECHARGE_RELAY_STATE_PIN,
    BOARD_QAC1_STATE_PIN,
    BOARD_QDC1_STATE_PIN,
    BOARD_CAC1_STATE_PIN,
    BOARD_CDC1_STATE_PIN};

/***********************************************************************
 * board_isValidPin(pin)
 *
 * Entrada:     pin         Pin digital a analizar
 *
 * Salida:      No posee
 *
 * Retorno:     1 si es un pin digital valido, 0 caso contrario
 *
 * Descripcion: Permite verificar que un pin es una entrada o salida
 *              digital valida.
 **********************************************************************/

int board_isValidPin(int pin)
{
    int i;
    for (i = 0; i < valid_pins_count; i++) {
        if (pin == valid_pins[i])
            return 1;
    }

    return 0;
}

/***********************************************************************
 * board_isCommandPin(pin)
 *
 * Entrada:     pin         Pin digital a analizar
 *
 * Salida:      No posee
 *
 * Retorno:     1 si es un pin digital de salida, 0 caso contrario
 *
 * Descripcion: Permite verificar que un pin es una salida digital
 **********************************************************************/

int board_isCommandPin(int pin)
{
    int i;
    for (i = 0; i < command_pins_count; i++) {
        if (pin == command_pins[i])
            return 1;
    }

    return 0;
}

/***********************************************************************
 * board_isStatePin(pin)
 *
 * Entrada:     pin         Pin digital a analizar
 *
 * Salida:      No posee
 *
 * Retorno:     1 si es un pin de estado de un contacto, 0 caso 
 *              contrario
 *
 * Descripcion: Permite verificar que un pin es de un estado de contacto
 **********************************************************************/

int board_isStatePin(int pin)
{
    int i;
    for (i = 0; i < state_pins_count; i++) {
        if (pin == state_pins[i])
            return 1;
    }

    return 0;
}

/***********************************************************************
 * board_digitalWrite(int pin, int state)
 *
 * Entrada:     pin         Pin digital sobre el que se actuara
 *              state       Estado deseado (1 o 0)
 *
 * Salida:      No posee
 *
 * Retorno:     0 si hubo exito, -1 caso contrario
 *
 * Descripcion: Permite modificar el estado de un pin digital de salida
 **********************************************************************/
int board_digitalWrite(int pin, int state)
{
    volatile Uint32 *reg;
    reg = NULL;

    if (!board_isCommandPin(pin))
        return -1;

    if (pin < 32) {
        reg = state ? &GpioDataRegs.GPASET.all : &GpioDataRegs.GPACLEAR.all;
    }
    else if (pin < 64) {
        reg = state ? &GpioDataRegs.GPBSET.all : &GpioDataRegs.GPBCLEAR.all;
        pin = pin - 32;
    }
    else if (pin < 88) {
        reg = state ? &GpioDataRegs.GPCSET.all : &GpioDataRegs.GPCCLEAR.all;
        pin = pin - 64;
    }

    // Se puede hacer asignación directa, ya que escribir un 0 no hace nada
    *reg = (1UL << pin);

    return 0;
}

/***********************************************************************
 * board_digitalRead(pin, state)
 *
 * Entrada:     pin         Pin digital a consultar
 *
 * Salida:      state       Estado del pin
 *
 * Retorno:     0 si hubo exito, -1 caso contrario
 *
 * Descripcion: Permite consultar el estado de un pin digital, sea 
 *              entrada o salida.
 **********************************************************************/
int board_digitalRead(int pin, int *state)
{
    volatile Uint32 *reg;
    reg = NULL;

    if (!board_isValidPin(pin))
        return -1;

    if (pin < 32) {
        reg = &GpioDataRegs.GPADAT.all;
    }
    else if (pin < 64) {
        reg = &GpioDataRegs.GPBDAT.all;
        pin = pin - 32;
    }
    else if (pin < 88) {
        reg = &GpioDataRegs.GPCDAT.all;
        pin = pin - 64;
    }

    (*state) = 0x00000001 & ((*reg) >> pin);

    return 0;
}

/***********************************************************************
 * board_openContact(pin)
 *
 * Entrada:     pin         Pin de comando
 *
 * Salida:      No posee
 *
 * Retorno:     0 si hubo exito, -1 caso contrario
 *
 * Descripcion: Wrapper de digitalWrite para facilitar los comandos de 
 *              apertura de los contactores y reles.
 **********************************************************************/

int board_openContact(int pin)
{
    return board_digitalWrite(pin, 1);
}

/***********************************************************************
 * board_openContact(pin)
 *
 * Entrada:     pin         Pin de comando
 *
 * Salida:      No posee
 *
 * Retorno:     0 si hubo exito, -1 caso contrario
 *
 * Descripcion: Wrapper de digitalWrite para facilitar los comandos de 
 *              cierre de los contactores y reles.
 **********************************************************************/

int board_closeContact(int pin)
{
    return board_digitalWrite(pin, 0);
}

/***********************************************************************
 * board_isClosed(pin)
 *
 * Entrada:     pin         Pin de estado del contacto
 *
 * Salida:      No posee
 *
 * Retorno:     1 si el contacto esta cerrado, 0 si esta abierto o 
 *              -1 si el pin no es valido.
 *
 * Descripcion: Indica si un pin de estado de un contacto (termica o 
 *              contactor) se encuentra en el estado correaspondiente
 *              a un contacto cerrado.
 **********************************************************************/

int board_isClosed(int pin)
{
    int level;
    int result;

    if (board_isStatePin(pin))
        return -1;

    result = board_digitalRead(pin, &level);

    if (result != 0)
        return -1;

    return level == BOARD_CONTACT_CLOSED_LEVEL ? 1 : 0;
}

/***********************************************************************
 * board_isOpened(pin)
 *
 * Entrada:     pin         Pin de estado del contacto
 *
 * Salida:      No posee
 *
 * Retorno:     1 si el contacto esta abierto, 0 si esta cerrado o 
 *              -1 si el pin no es valido.
 *
 * Descripcion: Indica si un pin de estado de un contacto (termica o 
 *              contactor) se encuentra en el estado correaspondiente
 *              a un contacto abierto.
 **********************************************************************/

int board_isOpened(int pin)
{
    int level;
    int result;

    if (board_isStatePin(pin))
        return -1;

    result = board_digitalRead(pin, &level);

    if (result != 0)
        return -1;

    return level == BOARD_CONTACT_OPEN_LEVEL ? 1 : 0;
}
