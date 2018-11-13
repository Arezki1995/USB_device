#include<stdio.h>
#include<stdlib.h>
#include<libusb-1.0/libusb.h>

/*
Device Found @ (Bus:Address) 2:2
Vendor ID 0x01c4f
Product ID 0x034

I am trying to detect my USB mouse

WE COULD USE 
-------------------------------------------------------------------------------------
libusb_device_handle* libusb_open_device_with_vid_pid 	( 	libusb_context *  	ctx,
															uint16_t  	vendor_id,
															uint16_t  	product_id ) 	

-------------------------------------------------------------------------------------
to search but this function has limitations that may constrain us

*/
/////////////////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
libusb_device_handle* myDeviceHandle = NULL;
libusb_context* context  = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////
void detectDevice( int myIdVendor, int myIdProduct, libusb_device_handle* myDeviceHandle){
	
	printf("Searching Device:\nIdVendor:0x0%x\t\tIdProduct:0x0%x\n\n",myIdVendor,myIdProduct);
	



	//Device List declaration & init
	libusb_device** list;
	ssize_t count=libusb_get_device_list(context,&list);
	
	//Device List initStatus verification
	if(count<0){
		perror("libusb_get_device_list"); 
		exit(-1);
	}
	ssize_t i=0;
	for(i=0;i<count;i++){
		//Declaring device list type
	 	libusb_device* device=list[i];
	 	
	 	//Declaring device descriptor structure
	  	struct libusb_device_descriptor desc;
	  	
	  	//Initialize device descriptor structure
	  	int status=libusb_get_device_descriptor(device,&desc);
	  	
	  	//init status verification
	  	if(status!=0){
	  		continue;
	  	}

			//device bus number & address init
	  		uint8_t bus=libusb_get_bus_number(device);
	  		uint8_t address=libusb_get_device_address(device);
	  	
	  		//Searching Specific device Vendor and product
	  		if(desc.idProduct==myIdProduct && desc.idVendor==myIdVendor){
	  			printf("Device Found @ (Bus:Address) %d:%d\n",bus,address);
	  			

	  			//Obtaining found device handle
	  			int openStatus= libusb_open ( device, &myDeviceHandle);
	  			
	  			//Open status verification
	  			if(openStatus !=0){
					perror("libusb_open()");
					exit(-1);  				
	  			}	  			

	
	  		}
	  }
	
	libusb_free_device_list(list,1);
	
}
////////////////////////////////////////////////////////////////////////////////////////

void printConfigDescriptor(struct libusb_config_descriptor* configDesc){
	printf("INDEX 0 CONFIGURATION\n");
	printf("bLength:\t%d\n",configDesc->bLength);
	printf("bLength:\t%d\n",configDesc->bDescriptorType);
	printf("bLength:\t%d\n",configDesc->wTotalLength);
	printf("bNumInterfaces:\t%d\n",configDesc->bNumInterfaces);
	printf("bConfigurationValue:\t%d\n",configDesc->bConfigurationValue);
	printf("iConfiguration:\t%d\n",configDesc->iConfiguration);
	printf("bmAttributes:\t%d\n",configDesc->bmAttributes);
	printf("MaxPower:\t%d\n",configDesc->MaxPower);
	
	//FIELDS TO PRINTF
	//const struct libusb_interface* interface
	//const unsigned char* extra
	//int extra_length
}
////////////////////////////////////////////////////////////////////////////////////////
void configure(libusb_device_handle* myDeviceHandle){
	
	//Declare & Initialize configuration Descriptor
	libusb_device* 	myDevice	= libusb_get_device(myDeviceHandle);
	struct libusb_config_descriptor* 	configDesc;
	
	//Getting configuration of index 0
	uint8_t	index = 0;
	int ConfigStatus	= libusb_get_config_descriptor(myDevice, index ,&configDesc); 
	if(ConfigStatus!= 0){
		perror("libusb_get_config_descriptor()");
		exit(-1);
	}
	//printConfigDescriptor(configDesc);
	
}


////////////////////////////////////////////////////////////////////////////////////////
int main(){
	
	int myIdVendor = 0x01c4f;
	int myIdProduct = 0x034;
	
	//Context type init
	int status=libusb_init(&context);
	
	//Context init Status verification
	if(status!=0) {
		perror("libusb_init"); 
		exit(-1);
	}

	detectDevice( myIdVendor, myIdProduct,myDeviceHandle);
	
	configure(myDeviceHandle);
	
	//exit context
	libusb_exit(context);
	
	return 0;
}
