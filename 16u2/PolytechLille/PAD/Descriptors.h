#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	/* Includes: */
		
		//PAR DEFAUT
		//#include <avr/pgmspace.h>
		//#include <LUFA/Drivers/USB/USB.h>
		
		//POUR COMPILER SUR MON PC
		#include "/usr/lib/avr/include/avr/pgmspace.h"
		#include "../../LUFA/Drivers/USB/USB.h"

	/* Type Defines: */
		/** Type define for the device configuration descriptor structure. This must be defined in the
		 *  application code, as the configuration descriptor contains several sub-descriptors which
		 *  vary between devices, and which describe the device's usage to the host.
		 */
		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;
			
			// LED INTERFACE
			USB_Descriptor_Interface_t            PADInterfaceLED;
				USB_Descriptor_Endpoint_t             IN_Endpoint_1;
				USB_Descriptor_Endpoint_t             IN_Endpoint_2;
			
			// BTN INTERFACE
			USB_Descriptor_Interface_t            PADInterfaceBTN;
				USB_Descriptor_Endpoint_t             OUT_Endpoint_1;
				USB_Descriptor_Endpoint_t             OUT_Endpoint_2;

		} USB_Descriptor_Configuration_t;

		/** Enum for the device interface descriptor IDs within the device. Each interface descriptor
		 *  should have a unique ID index associated with it, which can be used to refer to the
		 *  interface from other descriptors.
		 */
		enum InterfaceDescriptors_t
		{
			INTERFACE_ID_LED = 0, /* interface for LED descriptor ID */
			INTERFACE_ID_BTN = 1, /* interface for BTN descriptor ID */
		};

		/** Enum for the device string descriptor IDs within the device. Each string descriptor should
		 *  have a unique ID index associated with it, which can be used to refer to the string from
		 *  other descriptors.
		 */
		enum StringDescriptors_t
		{
			STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
			STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
			STRING_ID_Product      = 2, /**< Product string ID */
		};

	/* Macros: */

		/** Endpoint address of the PAD IN endpoints. (device to host) */
		//BTN joystick
		#define PAD_IN_1_EPADDR        (ENDPOINT_DIR_IN  | 1)
		//Other Btns choosed 8 to have a distinct addresse 
		#define PAD_IN_2_EPADDR        (ENDPOINT_DIR_IN  | 2)

		/** Endpoint address of the PAD OUT endpoints. (host to device) */
		//LED pin 13
		#define PAD_OUT_1_EPADDR       (ENDPOINT_DIR_OUT | 3)
		//other LEDs
		#define PAD_OUT_2_EPADDR       (ENDPOINT_DIR_OUT | 4)

		/** Size in bytes of the PAD IN and OUT endpoints. */
		#define PAD_EPSIZE           8


	/* Function Prototypes: */
		uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
		                                    const uint16_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);
#endif
