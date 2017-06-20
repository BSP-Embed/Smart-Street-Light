#include "main.h"

//#define TESTING

#ifdef TESTING
	const int8u PhNum[] = "9980237552";
#else
	const int8u PhNum[] = "8095392906";
#endif

enum {DAY,DUSK,NIGHT};

int8u POM[5];
int8u lflag[] = {0,0,0,0,0,0};
int8u  fault[NLIT];
int8u mode = DUSK;
int8u duskflag = 0;
int8u duskcnt = 0;

const char defstr[] = "Fault occurred in street light";


int main(void)
{
	int8u i;
	int8u dayflag = 0;
	int8u nitflag = 0;
	char str[2];
	char msg[50] = "";
	int8u fflag[NLIT];

	for (i = 0; i < NLIT; i++)
		fflag[i] = 0; 
	

	init();

	while (TRUE) {
		if (bit_is_clear(SENSOR_PIN, LDR_PIN)) {
			if (mode == DUSK) {
				if(duskflag == 0) {
					duskflag = 1;
					dayflag = 0;
					nitflag = 0;
					beep(1,75);
					lcdclrr(1);
					lcdws("   Dusk Time");
					StrtLit1On();
					StrtLit2On();
					StrtLit3On();
					StrtLit4On();
					StrtLit5On();
					duskcnt = 0;
				}
			} else {
				if (nitflag == 0) {
					beep(1,75);
					lcdclrr(1);
					lcdws("   Night Time");
					dayflag = 0;
					nitflag = 1;
					POM[0] = POM[1] = POM[2] = LOW_INT;
					POM[3] = POM[4] = POM[5] = LOW_INT;
					StartTmr();
				}
				if (bit_is_set(SENSOR_PIN, PIR1_PIN)) {
					 POM[0] = POM[1] = HIGH_INT;
				} else {
					lflag[0] = lflag[1] = 1;
				}
				if (bit_is_set(SENSOR_PIN, PIR2_PIN)) {
					 POM[1] = POM[2] = HIGH_INT;
				} else {
					lflag[1] = lflag[2] = 1;
				}
				if (bit_is_set(SENSOR_PIN, IR1_PIN)) {
					 POM[2] = POM[3] = HIGH_INT;
				} else {
					lflag[2] = lflag[3] = 1;
				}
				if (bit_is_set(SENSOR_PIN, IR2_PIN)) {
					 POM[3] = POM[4] = HIGH_INT;
				} else {
					lflag[3] = lflag[4] = 1;
				}
			}
			if (faultdet()) {
			
				for (i = 0; i < NLIT; i++)
					if (fault[i] == 1 && fflag[i] == 0) {
						str[0] = '0' + i + 1;
						str[1] = '\0';
						fflag[i] = 1;
						strcat (msg,defstr);
						strcat (msg, str);
						lcdclrr(1);
						lcdws(" Fault Detected");
						beep(1,75);
						GSMsndmsg(PhNum, msg);
						msg[0] = '\0';	/* NULL */
						fflag[i] = 1;
						dlyms(500);
						disptitl();
					}
			}
		} else {
			if (dayflag == 0) {
				StopTmr();
				StrtLitOff();
				beep(1,75);
				lcdclrr(1);
				lcdws("    Day Time");
				dayflag = 1;
				mode = DUSK;
				duskflag = 0;
			}
		} 
	
	}
	return 0;
}
static void init(void)
{
	BUZ_DDR 		|= _BV(BUZ_PIN);
	LED_DDR 		|= _BV(LED_PIN);

	FAULT_DDR 		&= ~0xF1;
	FAULT_PORT	 	|= 0xF1;

	STRT_LIT_DDR 	= 0xff;
	STRT_LIT_PORT 	= 0x00;

	SENSOR_DDR &= ~(0xfc);
	SENSOR_PORT |= 0xfc;
	
	beep(1,75);
	lcdinit();
	uartinit();
	GSMinit();
	disptitl();
	tmr1init();
	tmr0init();
	sei();
}
static void disptitl (void)
{ 
	lcdclr();
	lcdws("Intelli Strt Lit");
}

static void tmr1init(void)
{
	TCNT1H   = 0xD3;
	TCNT1L   = 0x00;
	TIMSK   |= _BV(TOIE1);			//ENABLE OVERFLOW INTERRUPT
	TCCR1A   = 0x00;					
	TCCR1B  |= _BV(CS10) | _BV(CS11); /* PRESCALAR BY 16 */
}
static void tmr0init(void)
{
	TCNT0   =  37;
	TIMSK   |= _BV(TOIE0);			//ENABLE OVERFLOW INTERRUPT
	
}
ISR(TIMER1_OVF_vect) 
{ 
	static int8u i,j;

	TCNT1H = 0xD3;
	TCNT1L = 0x00;
	
	for (i = 0; i < 6; i++) {
		if (lflag[i]) {
			if (POM[i] > LOW_INT) POM[i]--;
			else lflag[i] = 0;
		}
	} 
	if (++j >= 50) j = 0;
	switch(j) {
		case 0: case 2: ledon();  break;
		case 1: case 3: ledoff(); break;
	} 
	if (duskflag && (duskcnt++ >= 100)) {
		mode = NIGHT;
		duskcnt = 0;
	}		
}

ISR(TIMER0_OVF_vect) 
{ 
	static int8u i;
	TCNT0 = 37;
	

	if(i++ == 100)
		i = 0;
	if(i < POM[0]) StrtLit1On();
	else StrtLit1Off();
	if(i < POM[1]) StrtLit2On();
	else StrtLit2Off();
	if(i < POM[2]) StrtLit3On();
	else StrtLit3Off();
	if(i < POM[3]) StrtLit4On();
	else StrtLit4Off();
	if(i < POM[4]) StrtLit5On();
	else StrtLit5Off();

}
static int8u faultdet(void)
{
	int8u i,time = 0;

	for (i = 0; i < NLIT; i++)
		fault[i] = 1;

	for (i = 0; i < NLIT; i++) {
		time = 0;
		while (time++ < 40) {
			if ((FAULT_SEN & (0x08 << i)) == 0) {
				fault[i] = 0;
				break;
			}
			_delay_us(100);
		}
	}
	for (i = 0; i < NLIT; i++)
		if (fault[i] == 1) 
			return 1;

	return 0;
}
	
