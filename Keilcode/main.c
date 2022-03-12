#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "UART.h"
#include "LCD1602.h"
#include "Key.h"
#include "Music.h"

sbit Buzzer=P2^1; //蜂鸣器端口定义

unsigned char FreqSelect,MusicSelect,ScaleNum,Music_Play,KeyNum;

/**
  * @brief  根据按键的数字确认音调
  * @param  按键数字
  * @retval 对应的音调
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
		if(Lock==0&&KeyNum)       //如果没有解锁按键被检测到按下
		{
			
			//密码模块
			if(KeyNum<=6)	         //输入密码 密码为六进制
			{
				if(Count<4)	
				{
					Password*=10;				
					Password+=KeyNum%10;		
					Count++;	
				}
				LCD_ShowNum(2,1,Password,4);	
			}
			if(KeyNum==7)	        //确认按下 按键7为确认键
			{
				if(Password==2345)	
				{
					LCD_ShowString(1,14,"OK ");	
					Lock=1;                         
					Password=0;		
					Count=0;		
					LCD_ShowNum(2,1,Password,4);	 //检测密码正确，计数清零，Lock标记解锁
				}
				else				
				{
					LCD_ShowString(1,14,"ERR");	
						for(i=0;i<250;i++)
					{
						Buzzer=!Buzzer;
						Delay(2);                       //检测密码错误，计数清零,蜂鸣器发出警报
					}
					Password=0;		
					Count=0;		
					LCD_ShowNum(2,1,Password,4);	
				}
			}
			if(KeyNum==8)	         //取消全部 按下按键8后password清零
			{
				Password=0;		
				Count=0;		
				LCD_ShowNum(2,1,Password,4);	
			}
		}
		//电子琴模块
		if(Lock==1&&KeyNum)           //解锁后检测到按键按下
		{
				Music_Play=0;
				LCD_ShowString(1,1,"Enjoy MUSIC!!");
				LCD_ShowString(2,1,"             ");
				LCD_ShowNum(2,1,KeyNum,2);	
				TR0=1;
				FreqSelect=Scale(KeyNum);         //KeyNum转化为对应的音调赋值给频率选择
				Delay(SPEED/4);	
				TR0=0;
		}
		//播放音乐模块
		if(Lock==1&&Music_Play==1)
		{
			LCD_ShowString(2,1,"Music time!!!");
			if(Music1[MusicSelect]!=0xFF)	
			{
				FreqSelect=Music1[MusicSelect];	      //通过MusicSelect取数组Music中的乐谱音调
				MusicSelect++;				
				Delay(SPEED/4*Music1[MusicSelect]);	  //通过MusicSelect取数组Music中的乐谱时值
				MusicSelect++;
				TR0=0;																
				Delay(5);	
				TR0=1;
			}
			else	
			{
				TR0=0;
				MusicSelect=0;                 //音乐结束，MusicSelect清零
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



void Timer0_Routine() interrupt 1 //根据选定频率开启定时器
{
	if(FreqTable[FreqSelect])	
	{
		TL0 = FreqTable[FreqSelect]%256;		 //配置低位定时器
		TH0 = FreqTable[FreqSelect]/256;		 //配置高位定时器
		Buzzer=!Buzzer;	
	}
}

void UART_Routine() interrupt 4     //蓝牙模块
{
	if(RI==1)					
	{		
		//蓝牙播放音乐
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
		//蓝牙模拟按键按下
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