#include<reg51.h>
sbit led1=P0^1;
sbit led2=P0^2;
sbit led3=P0^3;
unsigned char rx_buf[22];
unsigned char rx_index;
unsigned char tx_buf[22];
unsigned char tx_index;
unsigned char tx_flag;
void variable_init();
void led_init();
void receive_packets();
void send_packets();
void packet_process();   
void serial_init();

void main()
{
variable_init();
led_init();
serial_init();
	
while(1)
{
  receive_packets();
  packet_process();
	if(tx_flag==1)
	{
	send_packets();
} 
}
}

void variable_init()
{
  unsigned char i;
  rx_index=0;
	tx_index=0;
	tx_flag=0;
  for(i=0;i<22;i++)
{
	tx_buf[i]=0;
	rx_buf[i]=0;
}
}

void led_init()
{
	led1=0;
	led2=0;
	led3=0;
}
	
void serial_init()
{
  TMOD = 0X20;
	TH1 = 0XFD; 
	SCON = 0x50;     
	TR1 = 1;
	TI=0;
	RI=0;
}
void send_packets()
{
	SBUF = tx_buf[tx_index];
  tx_index++;
	while(TI==0); 		//Wait until Transmitted
  TI=0;
	if(tx_index>21)
	{
		tx_index=0;
		tx_flag=0;
	}
}

void receive_packets()
{
if(RI==1)
{
	rx_buf[rx_index] = SBUF;
	rx_index++;
	if(rx_index>21)
	{
	rx_index=0;
	tx_flag=1;
	}
	RI=0;
}
}

void packet_process() 
{
  switch(rx_buf[0])
{
  case 0xA1:
      led1=1;
      led2=0;
      led3=0;
			tx_buf[0]=0xA2;
      break;
	
   case 0xA3:
      led1=0;
      led2=1;
      led3=0;
			tx_buf[0]=0xA4;
      break;
	 
   case 0xA5:
      led1=0;
      led2=0;
      led3=1;
			tx_buf[0]=0xA6;
      break;
	 
   default:
      led1=0;
      led2=0;
      led3=0;
}
}
