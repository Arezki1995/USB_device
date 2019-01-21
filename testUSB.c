#include<stdio.h>
#include<stdlib.h>
#include<libusb-1.0/libusb.h>
#include<string.h>

#define CONFIG_INDEX 0

//DEVICE TO SEARCH 04f2:b3d8
#define MY_ID_VENDOR  0xf00f
#define MY_ID_PRODUCT 0x6666
#define MAX_EP_NUM 4
#define DEFAULT_TIMOUT 100
#define DATA_SIZE 1
/////////////////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
libusb_device_handle* myDeviceHandle = NULL;
struct libusb_config_descriptor* 	configDesc;
libusb_context* context  = NULL;
uint8_t Nb_of_endpoints = 0;
unsigned char endpointAddressArray[MAX_EP_NUM];

//////////////////////////////////////////////////////////////////////////////////////////////
void sendThroughEndpoint(int endpointAddr, unsigned char * data){
	int transferred;
	libusb_interrupt_transfer 	( myDeviceHandle, endpointAddr, data, DATA_SIZE, &transferred, 0 );
	printf("Transfered: %d bytes\n", transferred);
}


void detectDevice( int myIdVendor, int myIdProduct, libusb_device_handle** myDeviceHandle){
	
	printf("Searching Device:\n\tIdVendor:0x0%x\n\tIdProduct:0x0%x\n\n",myIdVendor,myIdProduct);
	



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
	  			printf("Device Found @ \n\t(Bus:Address) %d:%d\n\n",bus,address);
	  			

	  			//Obtaining found device handle
	  			int openStatus= libusb_open ( device, myDeviceHandle);
	  			
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
	printf("\tbLength\t\t\t:%d\n",configDesc->bLength);
	printf("\tbDescriptorType\t\t:%d\n",configDesc->bDescriptorType);
	printf("\twTotalLength\t\t:%d\n",configDesc->wTotalLength);
	printf("\tbNumInterfaces\t\t:%d\n",configDesc->bNumInterfaces);
	printf("\tbConfigurationValue\t:%d\n",configDesc->bConfigurationValue);
	printf("\tiConfiguration\t\t:%d\n",configDesc->iConfiguration);
	printf("\tbmAttributes\t\t:%d\n",configDesc->bmAttributes);
	printf("\tMaxPower\t\t:%d\n",configDesc->MaxPower);
	printf("\t...\n\n");
	//FIELDS THAT ARE NOT PRINTED
		//FIELD OF INTEREST TO GET THE USB INTERFACES
			//const struct libusb_interface* interface
	
		//const unsigned char* extra
		
		//int extra_length
	
}
////////////////////////////////////////////////////////////////////////////////////////
void configure(libusb_device_handle* myDeviceHandle){
	
	//Declare & Initialize configuration Descriptor
	libusb_device* 	myDevice	= libusb_get_device(myDeviceHandle);
	
	
	//Getting configuration at index 0;
	int getConfDescStatus=libusb_get_config_descriptor(myDevice,CONFIG_INDEX,&configDesc);
	printf("getConfDescStatus : %d\n",getConfDescStatus);
	if(getConfDescStatus!=0){
		perror("libusb_get_config_descriptor():");
	}
	printf("configDescriptor: %p\n\n",configDesc);

	//printing Some Config descriptor fields
	printConfigDescriptor(configDesc);
	
	const struct libusb_interface*  interfaceArray= configDesc->interface;
	printf("inteface array: 0x%p\n\n",interfaceArray);
	////////
	printf("Started detatching interfaces...\n");
	for(uint8_t i=0; i< configDesc->bNumInterfaces; i++){
		  		printf("\t> detatching interface %d \n",i);
				int detachStatus=libusb_detach_kernel_driver (myDeviceHandle, interfaceArray[i].altsetting[0].bInterfaceNumber);
				if(detachStatus!=0){
					perror("libusb_detach_kernel_driver()");
				}

	}
	printf("finished detaching interfaces...\n\n");	

	
	//SETTING INTERFACE
	printf("setting configuration of index %d ...\n\n",CONFIG_INDEX);
	int SetConfStatus=libusb_set_configuration ( myDeviceHandle, configDesc->bConfigurationValue);
	if(SetConfStatus!=0){
		perror("libusb_set_configuration()");
	}
	
	//Iterating ConfigDescriptor interfaces
	int NUM_ALT=0;
	printf("Started Claiming interfaces...\n");
	for(uint8_t i=0; i< configDesc->bNumInterfaces; i++){
		
		printf("\tInterface with index %d:\n",i);
			
			uint8_t interfaceNb = interfaceArray[i].altsetting[NUM_ALT].bInterfaceNumber;
			
			printf("\t\tclaiming interface number %d\n",interfaceNb);
			int claimStatus=libusb_claim_interface(myDeviceHandle, interfaceNb);	
			if(claimStatus!=0){printf("\tInterface %d:\n",i);
				perror("libusb_claim_interface():");
			}
		
			
	}
	printf("finished Claiming interfaces\n\n");


	//Getting configuration of index 0
	uint8_t	index = 0;
	int ConfigStatus	= libusb_get_config_descriptor(myDevice, index ,&configDesc); 
	
	if(ConfigStatus!= 0){
		
		perror("libusb_get_config_descriptor()");
		exit(-1);
	}


	printf("Getting endpoints\n");
	
	for(uint8_t i=0; i< configDesc->bNumInterfaces; i++){
			printf("\tInterface %d:\n",i);
			uint8_t NbEndPoints = interfaceArray[i].altsetting[NUM_ALT].bNumEndpoints;
			for(int k=0; k<NbEndPoints; k++){
				
				printf("\t\tEndpoint index %d:\n",i);
				const struct libusb_endpoint_descriptor endPointDesc= interfaceArray[i].altsetting[NUM_ALT].endpoint[k];				
				
				printf("\t\t\tDescriptor@\t\t: %p\n",	   &endPointDesc);
				printf("\t\t\tDescriptor Type\t\t: %d\n",	endPointDesc.bDescriptorType);
				printf("\t\t\tEndpoint@\t\t: %d\n",			endPointDesc.bEndpointAddress);
				printf("\t\t\tEP wMaxPacketSize\t: %d\n",	endPointDesc.wMaxPacketSize);
				
				int endpoint_transfer_type = endPointDesc.bmAttributes & 3 ; //keeping only the transfer types bits to compare 				
				if ( endpoint_transfer_type == 3){
					printf("found endpoint working through interuptions and stored.\t\t\n\n");					
					endpointAddressArray[Nb_of_endpoints] = endPointDesc.bEndpointAddress;
					Nb_of_endpoints ++;				
				} 
			}
	}
	

////
////
	printf("printing saved Endpoint addresses\n");
	for(int i=0 ; i<MAX_EP_NUM; i++ ){

		printf("[%d] %d\n",i , endpointAddressArray[i]);
	
	}
////
unsigned char data = 'a';
sendThroughEndpoint( endpointAddressArray[3],&data);



////

	//releasing interfaces and closing device
	printf("closing device : number of interfaces to release :   %d  \n", configDesc->bNumInterfaces );	
	for(uint8_t i=0; i< configDesc->bNumInterfaces; i++){
		  		printf("\t> releasing interface  %d ...\n",i);
				int detachStatus=libusb_release_interface (myDeviceHandle, interfaceArray[i].altsetting[0].bInterfaceNumber);
				if(detachStatus!=0){
					perror("libusb_detach_kernel_driver_2()");
				}
				printf("\t> interface n° %d released \n\n", interfaceArray[i].altsetting[0].bInterfaceNumber );
	} 
	
	
	


}
///////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////////
int main(){
	

	
	//Context type init
	int status=libusb_init(&context);
	
	//Context init Status verification
	if(status!=0) {
		perror("libusb_init"); 
		exit(-1);
	}

	detectDevice( MY_ID_VENDOR, MY_ID_PRODUCT,&myDeviceHandle);
	if(myDeviceHandle!=NULL){
		printf("DEV HANDLE: %p\n\n",myDeviceHandle);
	}
	else{
		printf("THE DEVICE YOU REQUIRED IS NOT CONNECTED TO PC\n");
		printf("\tHINT: plug in the device an restart program\n");
		exit(-1);
	}

	
	
	configure(myDeviceHandle);
	

	//close the handle

	libusb_close (myDeviceHandle);
	printf("device handle closed \n\n");	

	//exit context
	libusb_exit(context);
	
	return 0;
}
