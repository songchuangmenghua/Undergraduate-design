#include "usart.h"
#include "delay.h"

#define USART2_REC_LEN          70      
#define EN_USART2_RX			      1
#define USART2_RXBUFFERSIZE     1  

extern u8 USART2_RxBuffer[USART2_RXBUFFERSIZE];
extern u8 USART2_RX_BUF[USART2_REC_LEN]; 
extern u8 USART2_RX_STA;                 
extern UART_HandleTypeDef USART2_Handler; 


typedef struct
{
	float voltage;		    
	float current;		   
	float thrust;			
	float torque;     
	float epower;
	        
}HILtypeDef;


extern HILtypeDef HIL;
void METV53USART2_init(u32 bound);
u8 Tension_clear_Request(void);
u8 Throttle_Open_Request(void);
u8 Throttle_set_Request(u16 ThrottleDuty);
HILtypeDef METV53_Decode(u8 *RecData);
u8 Throttle_close_Request(void);



