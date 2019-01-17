

/** \file
 *
 *  Main source file for the RelayBoard program. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 */

#include "PAD.h"


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();
	
	GlobalInterruptEnable();

	for (;;)
	  USB_USBTask();
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

	/* Initialize Relays */
	DDRC  |=  ALL_RELAYS;
	PORTC &= ~ALL_RELAYS;
}


// il faut ajouter les init dans le setup hardware , par exemple : Serial init 













/
/** Event handler for the library USB Control Request reception event. */
/*
void EVENT_USB_Device_ControlRequest(void)
{
    const uint8_t SerialNumber[5] = { 0, 0, 0, 0, 1 };
	uint8_t ControlData[2]        = { 0, 0 };

    switch (USB_ControlRequest.bRequest)
	{
		case 0x09:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				LEDs_ToggleLEDs(LEDS_LED1);

				Endpoint_ClearSETUP();

				Endpoint_Read_Control_Stream_LE(ControlData, sizeof(ControlData));
				Endpoint_ClearIN();

				switch (USB_ControlRequest.wValue)
				{
					case 0x303:
						if (ControlData[1]) PORTC &= ~RELAY1; else PORTC |= RELAY1;
						break;
					case 0x306:
						if (ControlData[1]) PORTC &= ~RELAY2; else PORTC |= RELAY2;
						break;
					case 0x309:
						if (ControlData[1]) PORTC &= ~RELAY3; else PORTC |= RELAY3;
						break;
					case 0x30c:
						if (ControlData[1]) PORTC &= ~RELAY4; else PORTC |= RELAY4;
						break;
				}
			}

			break;
		case 0x01:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				LEDs_ToggleLEDs(LEDS_LED1);

				Endpoint_ClearSETUP();

				switch (USB_ControlRequest.wValue)
				{
					case 0x301:
						Endpoint_Write_Control_Stream_LE(SerialNumber, sizeof(SerialNumber));
						break;
					case 0x303:
						ControlData[1] = (PORTC & RELAY1) ? 2 : 3;
						break;
					case 0x306:
						ControlData[1] = (PORTC & RELAY2) ? 2 : 3;
						break;
					case 0x309:
						ControlData[1] = (PORTC & RELAY3) ? 2 : 3;
						break;
					case 0x30c:
						ControlData[1] = (PORTC & RELAY4) ? 2 : 3;
						break;
				}

				if (ControlData[1])
				  Endpoint_Write_Control_Stream_LE(ControlData, sizeof(ControlData));

				Endpoint_ClearOUT();
			}

			break;
	}
}

*/

/*

void send_report(){

	Endpoint_SelectEndpoint(PAD_IN_EPADDR1);
	
	if( Endpoint_IsReadWriteAllowed() && Serial_IsCharReceived){
		int data;
		Endpoint
*/

// est c qu'il faut ajouter EVENT_DEVICE_ connected et configuration changed




/** Event handler for the USB_ConfigurationChanged event. This is fired when the host sets the current configuration
 *  of the USB device after enumeration, and configures the keyboard device endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;
    
    /* Setup HID Report Endpoints */
    ConfigSuccess &= Endpoint_ConfigureEndpoint(PAD_EPADDR_IN1, EP_TYPE_INTERRUPT, PAD_EPSIZE, 1);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(PAD_EPADDR_IN2, EP_TYPE_INTERRUPT, PAD_EPSIZE, 1);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(PAD_EPADDR_OUT1, EP_TYPE_INTERRUPT, PAD_EPSIZE, 1);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(PAD_EPADDR_OUT2, EP_TYPE_INTERRUPT, PAD_EPSIZE, 1);
}







/** Handle IN EP */ // cas du maitre esclave
void Handle_EP_IN(void)
{
    static int ancien_etat_bouton;
    if ( Serial_IsCharReceived()){
		ancien_etat_bouton= Serial_ReceiveByte();
			}
	
    int bouton_pour_EP1 = ancien_etat_bouton & MASk  ; // mask adequat plus decalage
    int bouton_pour_EP2 = ancien_etat_bouton & MASk2  ; // mask adequat plus decalage
    
    Endpoint_SelectEndpoint(PAD_EPADDR_IN1);
    if (Endpoint_IsReadWriteAllowed())
    {
        Endpoint_Write_8(bouton_pour_EP1);
        Endpoint_ClearIN();
    }
    Endpoint_SelectEndpoint(PAD_EPADDR_IN2);
    if (Endpoint_IsReadWriteAllowed())
    {
        Endpoint_Write_8(bouton_pour_EP2);
        Endpoint_ClearIN();
    }
    
}

// deux cas : 
/** Handle IN EP */
void Handle_EP_IN(void)
{
    int bouton;
    int joystick;
    int data;
    static int ancien_etat_bouton;
    static int ancien_joystick = ancien_etat_bouton & MASK3 ; // decalage
    static int ancien_bouton = ancien_etat_bouton & MASK4 ; // decalage
    if ( Serial_IsCharReceived()){
	    data = Serial_ReceiveByte();
	    bouton = data & // mask plus decalage ;
	    joystick = data & // mask plus decalage ; 
	// comparaison des boutons :  
	}
     if ( ancien_joystik - joystick =!0 ) {
		Endpoint_SelectEndpoint(PAD_EPADDR_IN1);
   			 if (Endpoint_IsReadWriteAllowed())
 	  			 {
   				       Endpoint_Write_8(joystick);
					 Endpoint_ClearIN();	  
				}
	}
	if ( ancien_bouton - bouton =!0 ) {
		Endpoint_SelectEndpoint(PAD_EPADDR_IN2);
   			 if (Endpoint_IsReadWriteAllowed())
 	  			 {
   				       Endpoint_Write_8(bouton);
					 Endpoint_ClearIN();	  
				}
	}
}




/// faut adapter la led 13 = bon mask + decalage

// Handle OUT EP
void Handle_EP_OUT(void)
{
    uint8_t LEDReport;
    /* Select the Keyboard LED Report Endpoint */
    Endpoint_SelectEndpoint(PAD_EPADDR_OUT1);
    
    /* Check if Keyboard LED Endpoint contains a packet */
    if (Endpoint_IsOUTReceived())
    {
        /* Check to see if the packet contains data */
        if (Endpoint_IsReadWriteAllowed())
        {
            /* Read in the LED report from the host */
            LEDReport = Endpoint_Read_8() & 0x01;
            Serial_SendByte(LEDReport);
            
            /* Process the read LED report from the host */
        }
        /* Handshake the OUT Endpoint - clear endpoint and ready for next report */
        Endpoint_ClearOUT();
    }
    
    Endpoint_SelectEndpoint(PAD_EPADDR_OUT2);
    
    if (Endpoint_IsOUTReceived())
    {
        if (Endpoint_IsReadWriteAllowed())
        {
            /* Read in the LED report from the host */
            LEDReport = Endpoint_Read_8() & 0x01;
            Serial_SendByte(LEDReport);
        }
        Endpoint_ClearOUT();
    }
}

