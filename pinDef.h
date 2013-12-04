/*
 * pinDef.h
 *
 *  Created on: Apr 14, 2013
 *      Author: popai
 */

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.
// ATMEL ATMEGA8 & 168 / ARDUINO
//
//   		             +-\/-+
//      		   PC6  1|    |28  PC5 (AI 5)	jp3		cfgpc
//		     (D 0) PD0  2|    |27  PC4 (AI 4)	jp2		delEEPROM mode
//		     (D 1) PD1  3|    |26  PC3 (AI 3)	jp1		config mode
//OUT1       (D 2) PD2  4|    |25  PC2 (AI 2)	TMP3
//OUT2  PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)	TMP2
//OUT3       (D 4) PD4  6|    |23  PC0 (AI 0)	TMP1
//		           VCC  7|    |22  GND
//		           GND  8|    |21  AREF
//		           PB6  9|    |20  AVCC
//                 PB7 10|    |19  PB5 (D 13)		useLED
//OUT4  PWM+ (D 5) PD5 11|    |18  PB4 (D 12)		errLED
//OUT5  PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM	IN4
//OUT6       (D 7) PD7 13|    |16  PB2 (D 10) PWM	IN3
//IN1        (D 8) PB0 14|    |15  PB1 (D 9)  PWM	IN2
//                       +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)
// Do not remove the include below

#ifndef PINDEF_H_
#define PINDEF_H_


void pinSetUp();


#endif /* PINDEF_H_ */
