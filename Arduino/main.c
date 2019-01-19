#include <avr/io.h>		// for the input/output registers
#include <util/delay.h>
/*
PINOUT
------
LED conneceted to 8 to 13 pins corresponding to PORTB 0 - 5

BUTTONS:
	D6 ---> pin 6 ----> PORTD 6
	D5 ---> pin 5 ----> PORTD 5
	D4 ---> pin 4 ----> PORTD 4
	D3 ---> pin 3 ----> PORTD 3
	
	D2 ---> pin 2 ----> PORTD 2
	
*/
// For the serial port
#define CPU_FREQ        16000000L       // Assume a CPU frequency of 16Mhz
#define BAUD_RATE 9600   					//BAUDRATE
#define ON_MODE 0x60
#define OFF_MODE 0x40
#define _ON 1
#define _OFF 0

uint8_t etatBtn, old_etatBtn;
uint8_t etatSerial;
/////////////////////////////////////////////////////////////////////////
void init_serial(int speed)
{
	/* Set baud rate */
	UBRR0 = CPU_FREQ/(((unsigned long int)speed)<<4)-1;

	/* Enable transmitter & receiver */
	UCSR0B = (1<<TXEN0 | 1<<RXEN0);

	/* Set 8 bits character and 1 stop bit */
	UCSR0C = (1<<UCSZ01 | 1<<UCSZ00);

	/* Set off UART baud doubler */
	UCSR0A &= ~(1 << U2X0);
}

/////////////////////////////////////////////////////////////////////////
void send_serial(unsigned char c)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

/////////////////////////////////////////////////////////////////////////
unsigned char get_serial(void) {
	//loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

/////////////////////////////////////////////////////////////////////////
void init_io(){
	//output
	//DDRB |= 0x01; // PIN 8 as output
	DDRB |=0b00111111;

	//DDRD &= 0xfb;  // PIN 2 as input
	//PORTD |= 0x04; // Pull-up activated on PIN 2	
	DDRD  &= 0b10000011;
	PORTD |= 0b01111100;
}
//////////////////////////////////////////////////////////////////////////

// read Button
void readBtn(){
	//etatBtn = ( PIND&0x7C ); //0111 1100
	etatBtn = ( PIND&=0x7C );  // Lire les pins des boutons
	etatBtn = (etatBtn >> 2);  // redecaler
	etatBtn = etatBtn |= 0x20; // ajouter un 1 au bit 5
}

void setLED(uint8_t valeur){
	if(valeur=='a') PORTB |= 0x01;
	if(valeur=='b') PORTB |= 0x02;
	if(valeur=='c') PORTB |= 0x04;
	if(valeur=='d') PORTB |= 0x08;
	if(valeur=='e') PORTB |= 0x10;
	if(valeur=='f') PORTB |= 0x20;

	if(valeur=='A') PORTB &= 0xfe;
	if(valeur=='B') PORTB &= 0xfd;
	if(valeur=='C') PORTB &= 0xfb;
	if(valeur=='D') PORTB &= 0xf7;
	if(valeur=='E') PORTB &= 0xef;
	if(valeur=='F') PORTB &= 0xdf;

}


void reportBtn(uint8_t etat)
{	
	switch(etat){
		case '7':
				send_serial('1');
				break;
		case ';':
				send_serial('2');
				break;
		case '=':
				send_serial('3');
				break;
		case '>':
				send_serial('4');
				break;
		case '/':
				send_serial('5');
				break;
		default:
				//send_serial(etat);
				break;
	}	
}


int main(void){

	init_serial(BAUD_RATE);	
	init_io();
	
	while(1){
		etatSerial = get_serial();
		setLED(etatSerial);
		
		readBtn();
		
			//THIS LINE IS FOR THE MISSING BTN TO REMOVE LATER 
			etatBtn = etatBtn |= 0x10;

		if(old_etatBtn != etatBtn) {
			reportBtn( etatBtn);
			old_etatBtn = etatBtn;
		}	
		_delay_ms(20);
	
	}
return 0;
}
