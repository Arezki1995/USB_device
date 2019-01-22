#ifndef _RELAYBOARD_H_
#define _RELAYBOARD_H_

	/* Includes: */
		//#include <avr/io.h>
		//#include <avr/wdt.h>
		//#include <avr/power.h>
		//#include <avr/interrupt.h>

		#include "/usr/lib/avr/include/avr/io.h"
		#include "/usr/lib/avr/include/avr/wdt.h"
		#include "/usr/lib/avr/include/avr/power.h"
		#include "/usr/lib/avr/include/avr/interrupt.h"


		#include "Descriptors.h"
		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>

	/* Macros: */
    #define TURN_LED_ON   0x60
    #define TURN_LED_OFF  0x40

    #define LED_1 0x01
    #define LED_2 0x02
    #define LED_3 0x03
    #define LED_4 0x04
    #define LED_5 0x05
    #define LED_13 0x06
    
    #define VALID_LED13_REQUEST (TURN_LED_OFF | TURN_LED_ON | LED_13 )
    #define VALID_LED_REQUEST (TURN_LED_OFF| TURN_LED_ON | LED_1 | LED_2 | LED_3 | LED_4 | LED_5 )

    #define BTN_MASK  0x20
    #define BTN_1 (1<<0)
    #define BTN_2 (1<<1)
    #define BTN_3 (1<<2)
    #define BTN_4 (1<<3)
    #define JOYSTICK (1<<4)
    #define VALID_BUTTONS_STATE ( BTN_4 | BTN_3| BTN_2| BTN_1)
    
	/* Function Prototypes: */
		void SetupHardware(void);

    void EVENT_USB_Device_ConfigurationChanged(void);

    void sendStatusOtherButtons(void);
    void sendStatusJoystickBtn(void);

    void listenOrderOtherLeds(void);
    void listenOrderLed13(void);
    
    uint8_t getStatusOfButtons(void);
    void ProcessLedOrder(uint8_t LedRequest);

#endif