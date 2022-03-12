#include <REGX51.H>

unsigned char Music_Play,KeyNum;

void UART_Init()  //11.0592M 无误差 9600bit
{
	SCON=0x50;
	PCON |= 0x80;
	TMOD &= 0x0F;		
	TMOD |= 0x20;		
	TL1 = 0xFA;		
	TH1 = 0xFA;		
	ET1 = 0;		
	TR1 = 1;
	EA = 1;
	ES = 1;
}

void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}

void UART_Input()
{
		if(SBUF==0x01)
		{
			Music_Play=1;
		}
		if(SBUF==0x02)
		{
			Music_Play=2;
		}
		if(SBUF==0x03)
		{
			Music_Play=3;
		}
		if(SBUF==0x05)
		{
			KeyNum=1;
		}
		if(SBUF==0x06)
		{
			KeyNum=2;
		}
		if(SBUF==0x07)
		{
			KeyNum=3;
		}
		if(SBUF==0x08)
		{
			KeyNum=4;
		}
		if(SBUF==0x09)
		{
			KeyNum=5;
		}
		if(SBUF==0x0A)
		{
			KeyNum=6;
		}
		if(SBUF==0x0B)
		{
			KeyNum=7;
		}
		if(SBUF==0x0C)
		{
			KeyNum=8;
		}
}

/*串口中断函数模板
void UART_Routine() interrupt 4
{
	if(RI==1)
	{


		RI=0;
	}
}
*/