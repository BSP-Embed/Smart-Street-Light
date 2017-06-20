#ifndef MAIN_H
#define MAIN_H

#include "includes.h"

#define F_CPU			7327800l

#define	STRT_LIT_DDR	DDRA
#define	STRT_LIT_PORT	PORTA
#define	STRT_LIT1_PIN	PA0
#define	STRT_LIT2_PIN	PA1
#define	STRT_LIT3_PIN	PA2
#define	STRT_LIT4_PIN	PA3
#define	STRT_LIT5_PIN	PA4


#define SENSOR_PORT		PORTD
#define SENSOR_DDR		DDRD
#define SENSOR_PIN		PIND

#define PIR1_PIN		PD2
#define PIR2_PIN		PD3
#define IR1_PIN			PD4
#define IR2_PIN			PD5
#define LDR_PIN			PD6

#define FAULT_DDR		DDRC
#define FAULT_PORT		PORTC
#define FAULT_SEN		PINC

#define F1PIN			PC3
#define F2PIN			PC4
#define F3PIN			PC5
#define F4PIN			PC6
#define F5PIN			PC7


//DEFINE CONSTANT
#define	TRUE			1
#define NLIT			5

#define ZRO_INT			0
#define LOW_INT			5
#define HIGH_INT		100

//DEFINE MACROS
#define StrtLit1On()	STRT_LIT_PORT |= _BV(STRT_LIT1_PIN)
#define StrtLit2On()	STRT_LIT_PORT |= _BV(STRT_LIT2_PIN)
#define StrtLit3On()	STRT_LIT_PORT |= _BV(STRT_LIT3_PIN)
#define StrtLit4On()	STRT_LIT_PORT |= _BV(STRT_LIT4_PIN)
#define StrtLit5On()	STRT_LIT_PORT |= _BV(STRT_LIT5_PIN)


#define StrtLit1Off()	STRT_LIT_PORT &= ~_BV(STRT_LIT1_PIN)
#define StrtLit2Off()	STRT_LIT_PORT &= ~_BV(STRT_LIT2_PIN)
#define StrtLit3Off()	STRT_LIT_PORT &= ~_BV(STRT_LIT3_PIN)
#define StrtLit4Off()	STRT_LIT_PORT &= ~_BV(STRT_LIT4_PIN)
#define StrtLit5Off()	STRT_LIT_PORT &= ~_BV(STRT_LIT5_PIN)


#define StartTmr()		TCCR0  	|= _BV(CS00)
#define StopTmr()		TCCR0  	&= ~_BV(CS00)
#define StrtLitOff()	STRT_LIT_PORT = 0x00;


static void 	init		(void);
static void 	disptitl 	(void);
static void 	tmr1init	(void);
static void 	tmr0init	(void);
static int8u 	faultdet	(void);

#endif
