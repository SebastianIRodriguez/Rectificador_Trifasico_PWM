/**********************************************************************
 * File:        board.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Se definen los pines digitales implementados en el
 *              proyecto y las funciones para el manejo de dichos pines
 *
 * History:
 *   13/05/2026 - original
 **********************************************************************/
#ifndef BOARD_H
#define BOARD_H

#include "gpio.h"

// Los pines de comandos son salidas, los de estado entradas
#define BOARD_GPIO08_COMMAND_PIN          8  // Pin digital 8 (grupo A)
#define BOARD_GPIO09_COMMAND_PIN          9  // Pin digital 9 (grupo A)
#define BOARD_PRECHARGE_RELAY_COMMAND_PIN 10 // Comando del rele de precarga (grupo A)
#define BOARD_PRECHARGE_RELAY_STATE_PIN   15 // Estado del rele de precarga (grupo A)
#define BOARD_QAC1_STATE_PIN              16 // Estado de la termica de AC (grupo A)
#define BOARD_QDC1_STATE_PIN              18 // Estado de la termica de DC (grupo A)
#define BOARD_CAC1_STATE_PIN              22 // Estado del contactor de AC (grupo A)
#define BOARD_CDC1_COMMAND_PIN            32 // Comanda el contactor de CC (grupo B)
#define BOARD_IPM_COMMAND_PIN             34 // Comanda el rele de IPM (grupo B)
#define BOARD_CDC1_STATE_PIN              86 // Estado del contactor de CC (grupo C)
#define BOARD_CAC1_COMMAND_PIN            87 // Comanda el cotactor de CA (grupo C)

#define BOARD_CONTACT_CLOSED_LEVEL 0 // Nivel que se mide cuando un contacto esta cerrado
#define BOARD_CONTACT_OPEN_LEVEL   1 // Nivel que se mide cuando un contacto esta abierto

int board_isValidPin(int pin);
int board_isCommandPin(int pin);
int board_isStatePin(int pin);
int board_digitalWrite(int pin, int state);
int board_digitalRead(int pin, int *state);
int board_openContact(int pin);
int board_closeContact(int pin);
int board_isClosed(int pin);
int board_isOpened(int pin);
#endif
