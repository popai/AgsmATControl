/*
 * gsm.c
 *
 *  Created on: Apr 14, 2013
 *      Author: popai
 */

#include <avr/eeprom.h>
//#include <stdio.h>
//#include <util/delay.h>
//#include <string.h>
//#include <stdlib.h>
//#include <stdint.h>
#include "Arduino.h"
#include "gsm.h"

static void SetPort();
//#define DEBUG 0

//A common buffer used to read response from pfon
//char buffer[128];

const prog_char ATE0[] PROGMEM = "ATE0"; 				//< Echo off
const prog_char AT_CNMI[] PROGMEM = "AT+CNMI=1,1,0,0,1"; //< Identification of new sms
const prog_char AT_CPMS[] PROGMEM = "AT+CPMS=\"ME\""; 	//< Preferred storage
const prog_char AT_CMGF[] PROGMEM = "AT+CMGF=1"; //< Send SMS string format 0 = PDU, 1 = text mod
const prog_char CMTI[] PROGMEM = "+CMTI:";
const prog_char AT_CBC[] PROGMEM = "AT+CIND?";
const prog_char CBC[] PROGMEM = "+CIND:";
const prog_char OK[] PROGMEM = "OK";
//const prog_char ERROR[] PROGMEM = "ERROR";
const prog_char CMD_ERR[] PROGMEM = "Comanda ne scrisa";


int8_t AT_INIT()
{
	int retVal = 1;
	char buffer[24];
	//int adr = 486;

	SetPort();

	//write mesage in EEPPROM
/*
	eeprom_read_block(buffer, (int*) 486, 24);
	if (strlen(buffer) == 0)
	{
		strcpy_P(buffer, CMD_ERR);
		eeprom_write_block(buffer, (int*) 486, 24);
	}
*/
	strcpy_P(buffer, CMD_ERR);
	eeprom_update_block(buffer, (int*) 486, 24);

	//*buffer = 0x00;
	PORTB = (1 << PB4);
	digitalWrite(12, HIGH);

	delay(1000);

	//strcpy_P(buffer, (char*) pgm_read_word(&(AT[0])));
	strcpy_P(buffer, ATE0);
	Serial.println(buffer);

	delay(1000);

	SerialRead(buffer);
	if (strstr_P(buffer, OK) == 0)
		retVal = -1;
#ifdef DEBUG
	Serial.println(buffer);
	Serial.println(retVal);
#endif

	//PORTB = ~(1 << PB4);
	delay(1000);

	//strcpy_P(buffer, (char*) pgm_read_word(&(AT[1])));
	strcpy_P(buffer, AT_CNMI);
	Serial.println(buffer);

	//PORTB = (1 << PB4);
	delay(1500);

	SerialRead(buffer);
	if (strstr_P(buffer, OK) == 0)
		retVal = -2;
#ifdef DEBUG
	Serial.println(buffer);
	Serial.println(retVal);
#endif

	//PORTB = ~(1 << PB4);
	delay(1000);

	//strcpy_P(buffer, (char*) pgm_read_word(&(AT[2])));
	strcpy_P(buffer, AT_CPMS);
	Serial.println(buffer);

	//PORTB = (1 << PB4);
	delay(1000);
	//PORTB = ~(1 << PB4);

	SerialRead(buffer);
	if (strstr_P(buffer, OK) == 0)
		retVal = -3;
#ifdef DEBUG
	Serial.println(buffer);
	Serial.println(retVal);
#endif

	delay(1000);
	//PORTB = (1 << PB4);

	//strcpy_P(buffer, (char*) pgm_read_word(&(AT[3])));
	strcpy_P(buffer, AT_CMGF);
	Serial.println(buffer);

	delay(1000);

	SerialRead(buffer);
	if (strstr_P(buffer, OK) == 0)
		retVal = -4;
#ifdef DEBUG
	Serial.println(buffer);
	Serial.println(retVal);
#endif
	PORTB &= ~(1 << PB4);
	//digitalWrite(12, LOW);
	return retVal;
}


int WaitForMsg()
{
	int p = Serial.parseInt();
	if (p > 0)
		return p;
	else
		return 0;
}

int8_t DellSms(const int id)
{
	PORTB = (1 << PB4);
	delay(500);
	//String for storing the command to be sent
	char cmd[16];
	//Build command string
	sprintf_P(cmd, PSTR("AT+CMGD=%d"), id);

	//Send Command
	Serial.println(cmd);
	//uint8_t len = WaitForResponse(1000);
	delay(1000);
	SerialRead(cmd);
	//Check if the response is OK
	if (strstr_P(cmd, OK) != 0)
	{
		PORTB &= ~(1 << PB4);
		return 1;
	}
	else
	{
		PORTB &= ~(1 << PB4);
		PORTB = (1 << PB5);
		return 0;
	}
}


int8_t ReadSms(const int id, char* nrtel, char *msg)
{
	PORTB = (1 << PB4);
	//String for storing the command to be sent
	char cmd[16];
	char buffer[128];
	char *start;
	//Build command string
	sprintf_P(cmd, PSTR("AT+CMGR=%d"), id);
	delay(500);
	//Send Command
	Serial.println(cmd);

	delay(3000);
	SerialRead(buffer);
	if (strstr_P(buffer, OK) != 0)	//Read = OK
	{
		//ghet pfone number
		start = strchr(buffer, ',');
		start += 4;
		strncpy(nrtel, start, 10);
		nrtel[10] = 0x00;
		//ghet mesage
		start = strchr(start, '*');
		//start += 2;
		while(*start == '*')
			start++;
		int8_t i;
		for (i = 0; start[i] != '*'; ++i)
			msg[i] = start[i];
		msg[i] = 0x00;
		//strcpy(buffer, "\0");
		PORTB &= ~(1 << PB4);
		return 1;
	}
	PORTB &= ~(1 << PB4);
	PORTB = (1 << PB5);
	return 0;
}

void SendSms(const char *num, const char *msg)
{
	PORTB = (1 << PB4);
	delay(1000);

	char cmd[25];

	sprintf_P(cmd, PSTR("AT+CMGS=\"%s\""), num);

	//Send Command
	Serial.println(cmd);

	delay(1800);
	//SerialRead(cmd);
	Serial.print(msg);

	Serial.write(0x1A); // Ctrl+Z
	Serial.write(0x0D); // CR
	Serial.write(0x0A); // LF
	delay(1000);
	PORTB &= ~(1 << PB4);

}

/***********************************************************
 *					 SERIAL READ
 Function read data on serial line
 - @citit: char array to hold data
 **********************************************************/
int8_t SerialRead(char *citit)
{
	int i = 0;
	while (Serial.available() > 0)
	{
		char c = Serial.read();
		if ((c != '\r') && (c != '\n'))
		{
			citit[i++] = c;
		}
		else
			citit[i++] = '*';
	}
	if (i > 0)
		citit[i - 1] = '\0';
//return str;
	return i;
}

int8_t BateryFull()
{
	char flashstr[24];
	char *ch;
	int bat = 0;
	//strcpy_P(flashstr, (char*) pgm_read_word(&(AT[12])));
	strcpy_P(flashstr, AT_CBC);
	Serial.println(flashstr);
	delay(1000);

	SerialRead(flashstr);
	if (strstr_P(flashstr, CBC) != 0)
	{
		ch = strchr(flashstr,':');
		ch++;
		bat = atoi(ch);
		//Serial.println(bat);
	}
	if (bat == 5)
	{
		//digitalWrite(outD6, HIGH);
		PORTD |= (1<<PD7);
		//Serial.println("off");
		return 1;
	}

	else
	{
		//digitalWrite(outD6, LOW);
		PORTD &= ~(1<<PD7);
		//Serial.println("on");
		return 0;
	}
}

/**
 * @breaf set the port on startup
 *
 * @param none
 */
void SetPort()
{
	int i;
	int8_t byte;
	for (i = 379; i < 384; i++)
	{
		byte = eeprom_read_byte((uint8_t*) i);
		switch (i)
		{
		case 379:
			if (byte)
				PORTD |= (1 << PD2);
			else
				PORTD &= ~(1 << PD2);
			break;

		case 380:
			if (byte)
				PORTD |= (1 << PD3);
			else
				PORTD &= ~(1 << PD3);
			break;

		case 381:
			if (byte)
				PORTD |= (1 << PD4);
			else
				PORTD &= ~(1 << PD4);
			break;

		case 382:
			if (byte)
				PORTD |= (1 << PD5);
			else
				PORTD &= ~(1 << PD5);
			break;

		case 383:
			if (byte)
				PORTD |= (1 << PD6);
			else
				PORTD &= ~(1 << PD6);
			break;

		default: break;

		}

	}
}

