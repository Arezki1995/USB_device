
#include "PAD.h"
#include "../../LUFA/Drivers/Peripheral/Serial.h"
static uint8_t old_LedState;
static uint8_t old_BtnState;

int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();
	
	
	for (;;){
		USB_USBTask();
		Serial_SendByte('b');
	}
	 
}

/** Configures the board hardware and chip peripherals for the project's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#endif

	/* Hardware Initialization */
	USB_Init();
	Serial_Init(9600, false);

	
}
///////////////////////////////////////////////////////////
/////	PROCESS FUNCTIONS

//Pour la communication série avec l’ATMega328p,la bibliothèque LUFA offre ces fonctions:
 	//Serial_Init
	//Serial_SendByte
	//Serial_IsCharReceived
	//Serial_ReceiveByte.

void ProcessLedOrder(uint8_t LedRequest){
		//send to the Atmega328p the new state of LEDs if there is any change otherwise nothing

		if( Serial_IsSendReady() && (old_LedState != LedRequest) ){
			Serial_SendByte(LedRequest);
			old_LedState=LedRequest;
		}
}


uint8_t getStatusOfButtons(void){
		//send to the host the status of the Buttons recieved from the Atmega328p
		uint16_t  new_etatBtn = Serial_ReceiveByte();
			
		while(!Serial_IsCharReceived()){
				//wait until we are sure a char is recieved
		}
			
			// decomposition: (uint16_t) Serial_ReceiveByte();
			// uint16_t = high uint8_t +  low uint8_t
			// Not sure of which part contains the relevent data but I guess it's low (UDR0)
			// The info is not present in documentation Ask REDON
			 
			 uint8_t low = (uint8_t) new_etatBtn;
			 
				// IF YOU FIND OUT IT IS THE HIGH PORTION 
			 	// uint8_t high = (uint8_t) (new_etatBtn >> 8);

			if(low != old_BtnState){
				old_BtnState=low;
				return low;
			}
			
		//default
		return 0;
}

///////////////////////////////////////////////////////////
/////////// HOST to ATMEGA16U2 communication  /////////////
///////////////////////////////////////////////////////////

/*
	Reads the next LED 13 status from the host from the LED 13 data endpoint,
	if one has been sent. 
*/

void listenOrderLed13(void)
{
	/* Select the PAD LED 13 Endpoint */
	Endpoint_SelectEndpoint(PAD_OUT_1_EPADDR);

	/* Check if PAD LED 13 Endpoint contains a packet */
	if (Endpoint_IsOUTReceived())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Read in the LED 13 order from the host */
			uint8_t LedRequest = Endpoint_Read_8();

			/* Process if valid LED 13 request from the host */
			
			if(LedRequest & VALID_LED13_REQUEST){			
				ProcessLedOrder( LedRequest);
			}
		}

		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();
	}
}

////////
/*
	Reads the next status of the other LEDs 13 from the host from the second endpoint,
	if one has been sent. 
*/

void listenOrderOtherLeds(void)
{
	/* Select the PAD Other LEDS Endpoint */
	Endpoint_SelectEndpoint(PAD_OUT_2_EPADDR);

	/* Check if PAD Other LEDS Endpoint contains a packet */
	if (Endpoint_IsOUTReceived())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Read in the LEDS order from the host */
			uint8_t LedRequest  = Endpoint_Read_8();

			/* Process the read LED report from the host */
			if(LedRequest & VALID_LED_REQUEST )
			{
				ProcessLedOrder(LedRequest);
			}
		}
		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();
	}
}

///////////////////////////////////////////////////////////
/////////////  ATMEGA to HOST communication ///////////////
///////////////////////////////////////////////////////////

void sendStatusJoystickBtn(void)
{
	/* Select the PAD Joystick button Endpoint */
	Endpoint_SelectEndpoint(PAD_IN_1_EPADDR);
	
	/* Check if PAD Joystick button Endpoint is ready */
	if (Endpoint_IsINReady())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Sends  the Joystick button status To the host */
			uint8_t newStatus = getStatusOfButtons();
			
			//check if the code recieved has the form of BTN status by ANDing with the mask if it is it should return non-zero (true)
			//check if the joystick button is ON. if so, returns non-zero (true)
			//both conditions must be satisfied to send the button state to the host otherwise it does nothing
			if( (newStatus & BTN_MASK)  && (newStatus & JOYSTICK) ){
				Endpoint_Write_8(newStatus);
			}
		}

		/* Handshake the IN Endpoint - clear endpoint and ready for next use */
		Endpoint_ClearIN();
	}
}


void sendStatusOtherButtons(void)
{
	/* Select other PAD buttons Endpoint */
	Endpoint_SelectEndpoint(PAD_IN_2_EPADDR);

	/* Check if this Endpoint contains is ready */
	if (Endpoint_IsINReady())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Sends the state of the other buttons status To the host */
			uint8_t newStatus = getStatusOfButtons();
			//check if the code recieved has the form of BTN status by anding with the mask if it is it should return non-zero (true)
			//check if at least one of the buttons [1-4] is ON if so returns non-zero (true)
			//both conditions must be satisfied to send the button state to the host otherwise it does nothing 
			if( (newStatus & BTN_MASK) && (newStatus & VALID_BUTTONS_STATE) ){
				Endpoint_Write_8(newStatus);
			}

		}
		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearIN();
	}
}