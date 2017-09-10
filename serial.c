#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>

#include "standart_functions.h"
#include "serial.h"

char *receiver_serial;

#ifdef DEBUG_SERIAL														
	
	/*
	Biblioteca que implementa uma software serial através de qualquer I/O
	Funciona bem
	Tem de ser configurada antes de começar a utilizar
	*/
	#include "dbg_putchar.h"													

#endif

void uart_config(void)
{
	/*
	É um registrador que armazena o valor do baudrate
	Para descobrir o valor, é preciso olhar as tabelas continas nessa página: http://wormfood.net/avrbaudcalc.php
	Atualmente setado para 57600
	*/
	UBRR0H = 0x00;
	UBRR0L = 0x22;
	/*
	Selecionando o numero de bits de cada pacote para 8
	*/
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	/*
	Habilita o RX e o TX
	*/
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	
#ifdef INTERRUPT_SERIAL

	/*
	Setar os bits para habilitar a interrupção da serial para o RX
	*/
	BitSet(UCSR0B, RXCIE0);
	BitSet(UCSR0A, RXC0);
	
#endif
	
	/*
	Bit para setar o baudrate corretamente
	Mais informações ver: http://wormfood.net/avrbaudcalc.php 
	*/
	BitSet(UCSR0A, U2X0);

#ifdef DEBUG_SERIAL

	/*
	Inicia a rotina de debug 
	*/
	dbg_tx_init();

#endif
}


void write_byte_serial(const char byte)
{
	/*
	Espera o registrador de envio estar livre
	*/
	while( !( UCSR0A & (1<<UDRE0) ) );
	
	/*
	Envia o byte
	*/
	UDR0 = byte;
}

char read_byte_serial(void)
{
	uint16_t i = 50000;
	
	/*
	Checa i vezes se há algum caracter a ser recebido no registrador UDR0
	Caso não tenha, retorna \0

	Técnica utilizada para gerar um sistema de timeout
	*/
	do
	{
		if (UCSR0A & (1<<RXC0))
		{
			return UDR0;
		}
	}while(--i);
	
	return '\0';
}

void write_string_serial(char *str)
{

#ifdef DEBUG_SERIAL

	/*
	Rotina de debug 
	*/
	debug(str);
	
#endif

	/*	
	Envia a string, até que o próximo caractere seja '/0'
	*/ 
	do
	{
		write_byte_serial(*str);
	} while (*++str);
}

char *read_string_serial(void)
{
	static char receiver[RX_BUFF];
	char *temp;
	temp = receiver;

#ifndef INTERRUPT_SERIAL

	/*
	Espera até que o uart receba o primeiro caractere 
	*/
	while( !( UCSR0A & (1<<RXC0) ) );

#endif
	/*
	Recebe uma string recebida na uart até encontrar o caracter '/0'
	*/
	while((*temp = read_byte_serial()) != '\0')
	{
		++temp;
	}
	
#ifdef DEBUG_SERIAL

	/*
	Rotina de debug
	*/
	debug(receiver);
	
#endif
	
	return receiver;
}

#ifdef INTERRUPT_SERIAL

ISR(USART_RX_vect)
{	
	/*
	Carrega na variável receiver_serial a string recebida
	*/
	receiver_serial = read_string_serial();
}

#endif