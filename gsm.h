/*
 * gsm.h
 *
 *  Created on: Apr 14, 2013
 *      Author: popai
 */

#ifndef GSM_H_
#define GSM_H_

//Public Interface
int8_t	AT_INIT();
int8_t DellSms(const int id);
//int8_t WaitForMsg(char *id);
int WaitForMsg();
int8_t ReadSms(const int id, char* nrtel, char *msg);
void SendSms(const char *num, const char *msg);
int8_t BateryFull();
int8_t SerialRead(char *citit);

#endif /* GSM_H_ */
