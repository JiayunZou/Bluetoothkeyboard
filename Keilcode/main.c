#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "UART.h"
#include "LCD1602.h"
#include "Key.h"
#include "Music.h"

sbit Buzzer=P2^1; //�������˿ڶ���

unsigned char FreqSelect,MusicSelect,ScaleNum,Music_Play,KeyNum;

/**
  * @brief  ���ݰ���������ȷ������
  * @param  ��������
  * @retval ��Ӧ������
  */
unsigned char Scale(unsigned char ScaleNumber)
{
	unsigned char Scaleout;
	switch(ScaleNumber)               
	{
		    case 1:Scaleout=M1;break;
        case 2:Scaleout=M2;break;
        case 3:Scaleout=M3;break;
        case 4:Scaleout=M4;break;
        case 5:Scaleout=M5;break;
        case 6:Scaleout=M6;break;
        case 7:Scaleout=M7;break;
	      case 8:Scaleout=H1;break;	
	}
	return Scaleout;
}



void main()
{	
	unsigned int Password;
	unsigned char i,Count,Lock=0;
	LCD_Init();
	LCD_ShowString(1,1,"PASSWORD:");
	Timer0Init();
	UART_Init();
	while(1)
	{
		KeyNum=Key();
		ScaleNum=Scale(KeyNum);
		if(Lock==0&&KeyNum)       //���û�н�����������⵽����
		{
			
			//����ģ��
			if(KeyNum<=6)	         //�������� ����Ϊ������
			{
				if(Count<4)	
				{
					Password*=10;				
					Password+=KeyNum%10;		
					Count++;	
				}
				LCD_ShowNum(2,1,Password,4);	
			}
			if(KeyNum==7)	        //ȷ�ϰ��� ����7Ϊȷ�ϼ�
			{
				if(Password==2345)	
				{
					LCD_ShowString(1,14,"OK ");	
					Lock=1;                         
					Password=0;		
					Count=0;		
					LCD_ShowNum(2,1,Password,4);	 //���������ȷ���������㣬Lock��ǽ���
				}
				else				
				{
					LCD_ShowString(1,14,"ERR");	
						for(i=0;i<250;i++)
					{
						Buzzer=!Buzzer;
						Delay(2);                       //���������󣬼�������,��������������
					}
					Password=0;		
					Count=0;		
					LCD_ShowNum(2,1,Password,4);	
				}
			}
			if(KeyNum==8)	         //ȡ��ȫ�� ���°���8��password����
			{
				Password=0;		
				Count=0;		
				LCD_ShowNum(2,1,Password,4);	
			}
		}
		//������ģ��
		if(Lock==1&&KeyNum)           //�������⵽��������
		{
				Music_Play=0;
				LCD_ShowString(1,1,"Enjoy MUSIC!!");
				LCD_ShowString(2,1,"             ");
				LCD_ShowNum(2,1,KeyNum,2);	
				TR0=1;
				FreqSelect=Scale(KeyNum);         //KeyNumת��Ϊ��Ӧ��������ֵ��Ƶ��ѡ��
				Delay(SPEED/4);	
				TR0=0;
		}
		//��������ģ��
		if(Lock==1&&Music_Play==1)
		{
			LCD_ShowString(2,1,"Music time!!!");
			if(Music1[MusicSelect]!=0xFF)	
			{
				FreqSelect=Music1[MusicSelect];	      //ͨ��MusicSelectȡ����Music�е���������
				MusicSelect++;				
				Delay(SPEED/4*Music1[MusicSelect]);	  //ͨ��MusicSelectȡ����Music�е�����ʱֵ
				MusicSelect++;
				TR0=0;																
				Delay(5);	
				TR0=1;
			}
			else	
			{
				TR0=0;
				MusicSelect=0;                 //���ֽ�����MusicSelect����
			}
		}
		if(Lock==1&&Music_Play==2)
		{
			LCD_ShowString(2,1,"Music time!!!");
			if(Music2[MusicSelect]!=0xFF)	
			{
				FreqSelect=Music2[MusicSelect];	
				MusicSelect++;
				Delay(SPEED/4*Music2[MusicSelect]);	
				MusicSelect++;
				TR0=0;
				Delay(5);	
				TR0=1;
			}
			else	
			{
				TR0=0;
				MusicSelect=0;
			}
		}
		if(Lock==1&&Music_Play==3)
		{
			LCD_ShowString(2,1,"Music time!!!");
			if(Music2[MusicSelect]!=0xFF)	
			{
				FreqSelect=Music3[MusicSelect];	
				MusicSelect++;
				Delay(SPEED/4*Music3[MusicSelect]);	
				MusicSelect++;
				TR0=0;
				Delay(5);	
				TR0=1;
			}
			else	
			{
				TR0=0;
				MusicSelect=0;
			}
		}
	}
}



void Timer0_Routine() interrupt 1 //����ѡ��Ƶ�ʿ�����ʱ��
{
	if(FreqTable[FreqSelect])	
	{
		TL0 = FreqTable[FreqSelect]%256;		 //���õ�λ��ʱ��
		TH0 = FreqTable[FreqSelect]/256;		 //���ø�λ��ʱ��
		Buzzer=!Buzzer;	
	}
}

void UART_Routine() interrupt 4     //����ģ��
{
	if(RI==1)					
	{		
		//������������
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
		//����ģ�ⰴ������
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
		UART_SendByte(SBUF);
		RI=0;
	}
}