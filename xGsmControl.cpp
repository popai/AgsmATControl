/*
 * gsmControl.cpp
 *
 *  Created on: Feb 4, 2013
 *      Author: popai
 */

// Do not remove the include below
#include "xGsmControl.h"
#include "pinDef.h"
#include "cmd.h"
#include "gsm.h"
#include "adc.h"

//#define DEBUG 1

const prog_char citeste[] PROGMEM = "citeste";

//The setup function is called once at startup of the sketch

bool config = false, delEEPROM = false, cfgpc = false;
int contor = 5000;

char read[24];
char teln[10];
//int id;

void setup()
{

// Add your initialization code here
	pinSetUp();
	InitADC();
	Serial.begin(9600);
	delay(100);
#ifdef DEBUG
	//Serial.println("-----------------------------------");
	Serial.println("PP");
	//Serial.println("-----------------------------------");
#endif
	PORTB = (1 << PB4);
	if (AT_INIT() != 1)
		PORTB = (1 << PB5);
	//PORTB &= ~(1 << PB4);
	//digitalWrite(errLED, HIGH);

	//EEPROM.setMaxAllowedWrites(2 * EEPROMSizeATmega168);
	//EEPROM.setMemPool(0, EEPROMSizeATmega168);

	//if (digitalRead(jp1) == LOW)
	if ((PINC & (1 << PC3)) == 0)
	{
		config = true;
	}
	//if (digitalRead(jp2) == LOW)
	if ((PINC & (1 << PC4)) == 0)
	{
		delEEPROM = true;
		DellEprom();
	}
	//if (digitalRead(jp3) == LOW)
	if ((PINC & (1 << PC5)) == 0)
	{
		cfgpc = true;
	}
#ifdef DEBUG
	//Serial.println(config);
	//Serial.println(delEEPROM);
	//Serial.println(delPass);
#endif

}

// The loop function is called in an endless loop
void loop()
{
	//char read[24];
	//char teln[10];
	//int adr;
	int id;
	if (delEEPROM)
		return;
	if (contor == 5000)
	{
		contor = 0;
		BateryFull();
	}
	++contor;

	if (cfgpc)
	{
		SerialRead(read);
		delay(100);
		//if (strlen(read) != 0)
		//Serial.println(read);

		if (strstr_P(read, citeste) != 0)
		{
			for (int i = 0; i <= 512; i += 18)
				ReadEprom(read, i);
		}
		else if (strlen(read) != 0)
			CfgCmd(read);
		*read = 0x00;
	}
	else if (config)
	{
		//SerialRead(read);
		delay(100);
		id = WaitForMsg();
		//adr = id;
		if (id > 0)
		{
			ReadSms(id, teln, read);
			//Serial.println(teln);
			//Serial.println(read);
			//Serial.println(id);
			Config(teln, read);
			//Serial.println(id);
			//id = adr;
			DellSms(id);
		}
		*read = 0x00;
		id = 0;
	}
	else
	{
		VerificIN();
		id = WaitForMsg();
		if (id > 0)
		{
			ReadSms(id, teln, read);
			Comand(teln, read);
			DellSms(id);
		}
		*teln = 0x00;
		*read = 0x00;
		id = 0;
		Serial.println("AT");
	}
	delay(100);
}

