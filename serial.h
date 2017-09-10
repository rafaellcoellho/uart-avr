/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42, by Joerg Wunsch):
 * <rafaellcoellho@gmail.com> wrote this file.  As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you 
 * think this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

/*
	@file						serial.h
	@autor					Rafael Coelho <rafaellcoellho@gmail.com>
	@brief					Biblioteca para utilizar o periférico serial do Atmega 328p
	@details                
 */

#ifndef SERIAL_H_
	#define SERIAL_H_

	#define RX_BUFF 							500								// Tamanho do buffer de recepção

	//#define DEBUG_SERIAL													// Comente essa linha para desabilitar o debug 
	
	#define INTERRUPT_SERIAL												// Comente essa linha para desabilitar a interrupção
	
	extern char *receiver_serial;										// Variável que armazena a string que foi recebida pela uart

	void uart_config(void);		
	void write_byte_serial(const char byte);
	char read_byte_serial(void);
	void write_string_serial(char *str);
	char *read_string_serial(void);

#endif /* SERIAL_H_ */