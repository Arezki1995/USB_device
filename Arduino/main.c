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
POTS:
	Haut/Bas 		--->	Analog in 0 ---> PORTC[0]
	Gauche/Droite 	--->	Analog in 1 ---> PORTC[1]
	

*/
// For the serial port

#define CPU_FREQ        16000000L       // Assume a CPU frequency of 16Mhz
#define SPEED 9600   					//BAUDRATE


uint8_t etatBtn;
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
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

// For I/O handling (examples for PIN 8 as output and PIN 2 as input)

void output_init(void){
	//DDRB |= 0x01; // PIN 8 as output
	DDRB |=0b00111111;
}

void output_set(unsigned char value){
	if(value==0) PORTB &= 0xfe; else PORTB |= 0x01;
}

void input_init(void){
	//DDRD &= 0xfb;  // PIN 2 as input
	//PORTD |= 0x04; // Pull-up activated on PIN 2
	
	DDRD  &= 0b10000011;
	PORTD |= 0b01111100;
}

unsigned char input_get(void){
	return ((PIND&0x04)!=0)?1:0;
}

// read Button
void readBtn(){
	etatBtn = ( PIND&0x7C ); //0111 1000
}

void readSerial(){
	etatSerial = UDR0;
	 //PORTB&0x3F;
	
}


int main(void){

	init_serial(SPEED);
	input_init();
	output_init();
	while(1){
	etatSerial=get_serial();
	send_serial(etatSerial);
	switch (etatSerial){
			case '0':
					PORTB=0x00;
					break;
			case '1':
					PORTB|=0x01;	
					break;
			case '2':
					PORTB|=0x02;
					break;
			case '3':
					PORTB|=0x04;	
					break;
			case '4':
					PORTB|=0x08;
					break;
			case '5':
					PORTB|=0x10;	
					break;
			case '6':
					PORTB|=0x20;
					break;
			case '7':
					PORTB=0xFF;	
					break;	
			}
	PORTB=etatSerial;
	_delay_ms(10);
	}
	/*while(1){
		readBtn();

		
		if ( etatBtn == 0)
		{	
			//send_serial('0');
		}
		else
		{
			
			switch (etatBtn){
			case 't':
					send_serial('D');
					break;
			case '\\':
					send_serial('B');
					break;
			case '<':
					send_serial('G');
					break;
			case 'l':
					send_serial('H');
					break;	
			case 'x':
					send_serial('P');
					break;										
			default :
					send_serial('-');
			}
		
		}
		_delay_ms(80);
		
		etatBtn=0;
		
	}
	*/

	return 0;
}
