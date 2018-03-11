/*
//芯片型号 STC89C52
//晶振12MHZ
//时钟芯片型号DS1302 DIP8
*/

#include <reg52.h>
#include <stdio.h>
#include <intrins.h>

typedef   unsigned char uint8_t;
typedef   unsigned int uint16_t;

#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^7;
sbit LCD1602_RW=P2^5;
sbit LCD1602_RS=P2^6;

sbit SCLK= P3^6;
sbit IO  = P3^4;
sbit CE  = P3^5;

sbit SET     = P3^3;
sbit CHANGE  = P3^2;

typedef enum
{
  Y=0,//年
	M,  //月
	D,  //日
	h,  //小时
	m,  //分钟
	s   //秒
}TimeType;


void delay_ms(unsigned int t);
void delay_200us(uint16_t t);


void Ds1302_Init();
void Ds1302_SendByte(uint8_t adr,uint8_t date);
uint8_t Ds1302_ReceiveByte(uint8_t adr);
void Ds1302_SetTime(TimeType T,uint8_t TimeValue);
uint16_t Ds1302_GetTime(TimeType T);


void LcdWriteCom(uint8_t com);
void LcdWriteData(uint8_t dat);
void LcdInit();
void LcdShowChar(uint8_t y,uint8_t x,uint8_t ch);
void Lcd_showTime(TimeType T,uint16_t TimeValue);

uint8_t second=0,minutes=0,hours=0,days=0,months=0;
uint16_t years=0;





int main(){
	
	  uint16_t t=0;
    uint8_t loction,ISSET=0;
	
	  Ds1302_Init();//初始化DS1302 
	  LcdInit();  //LCD1602初始化
    
	
	  SET=1;
	  CHANGE=1;
	
    while(1)  
    {          
			second = Ds1302_GetTime(s);
			minutes= Ds1302_GetTime(m);
			hours  = Ds1302_GetTime(h);
			days   = Ds1302_GetTime(D);
			months = Ds1302_GetTime(M);
			years  = Ds1302_GetTime(Y);
			
			Lcd_showTime(s,second);
			Lcd_showTime(m,minutes);
			Lcd_showTime(h,hours);
			Lcd_showTime(D,days);
			Lcd_showTime(M,months);
			Lcd_showTime(Y,years);
			
		
			while(SET==0){
				ISSET=1;
			if(SET==0){
			Lcd_showTime(h,hours);	
			LcdShowChar(1,6,0xff);
			LcdShowChar(1,7,0xff);
			loction=s;delay_ms(500);
			
			}
			if(SET==0){
			Lcd_showTime(s,second);
		  LcdShowChar(1,3,0xff);
			LcdShowChar(1,4,0xff);
			loction=m;delay_ms(500);
			
			}
			if(SET==0){
			Lcd_showTime(m,minutes);
			LcdShowChar(1,0,0xff);
			LcdShowChar(1,1,0xff);
			loction=h;delay_ms(500);
			
			}
			
			}
			
			   if(ISSET){
			     while(SET==1){
					          if(CHANGE==0){
					             switch(loction){
												 case s:   second++; Lcd_showTime(s,second);  break;
					                            }
											 delay_ms(300);
					                       }
					              }
				 ISSET=0;
			           }
			
    }  
}

  
void delay_ms(unsigned int t)  
{  
    unsigned char a,b;  
    while(t--)  
    {  
      for(b=80;b>0;b--)  
      for(a=2;a>0;a--);  
    }  
}  

void delay_200us(uint16_t t){
  unsigned char a,b;  
    while(t--)  
    {  
      for(b=16;b>0;b--)  
      for(a=2;a>0;a--);  
    }  
}

void Ds1302_Init(){
  CE=0;
	IO=0;
  SCLK=0;	
	CE=1;
	Ds1302_SendByte(0x8e,0x00);
	Ds1302_SendByte(0x80,0x00);
	Ds1302_SendByte(0x82,0x00);
	Ds1302_SendByte(0x84,0x00);
	Ds1302_SendByte(0x86,0x00);
	Ds1302_SendByte(0x88,0x00);
	Ds1302_SendByte(0x8c,0x00);

}

void Ds1302_SendByte(uint8_t adr,uint8_t date){
  uint8_t i=0;
  SCLK=0;
	CE=1;
	for(i=0;i<8;i++){	
	
	IO=adr&0x01;
	adr>>=1;
	SCLK=1;
	delay_200us(1);
	SCLK=0;
	delay_200us(1);
	}
	
	for(i=0;i<8;i++){
	
	IO=date&0x01;
	date>>=1;
	SCLK=1;
	delay_200us(1);
	SCLK=0;
	delay_200us(1);

	}
	CE=0;
	SCLK=0;
}

uint8_t Ds1302_ReceiveByte(uint8_t adr){
    uint8_t date=0,i=0;
    SCLK=0;
  	CE=1;
	for(i=0;i<8;i++){	
	IO=adr&0x01;
	adr>>=1;
	SCLK=1;
	delay_200us(1);
	SCLK=0;
	delay_200us(1);
	}
	
	for(i=0;i<8;i++){
	date>>=1;
	if(IO==1)date|=0x80;
	SCLK=1;
	delay_200us(1);
	
	SCLK=0;
	delay_200us(1);
	}
	CE=0;
	SCLK=1;
	delay_200us(1);
	IO=0;
	delay_200us(1);
	IO=1;
		return date;
}

void Ds1302_SetTime(TimeType T,uint8_t TimeValue){

	switch(T){
		
		case s: 
			Ds1302_SendByte(0x81,TimeValue); 
		  break;
		case m: 
			Ds1302_SendByte(0x83,TimeValue); 
		  TimeValue=Ds1302_ReceiveByte(); break;
		case h: 
			Ds1302_SendByte(0x85,TimeValue); 
		  TimeValue=Ds1302_ReceiveByte(); break;
		
		case D: 
			Ds1302_SendByte(0x87,TimeValue); 
		  TimeValue=Ds1302_ReceiveByte(); break;
		case M: 
			Ds1302_SendByte(0x89,TimeValue); 
		  TimeValue=Ds1302_ReceiveByte(); break;
		case Y: 
			Ds1302_SendByte(0x8d,TimeValue); 
		  TimeValue=Ds1302_ReceiveByte(); break;
		
	}
	
}

uint16_t Ds1302_GetTime(TimeType T){
  uint16_t TimeValue;
	switch(T){
		
		case s: 
		  TimeValue=Ds1302_ReceiveByte(0x81); 
		  TimeValue=(TimeValue&0x0f)+10*((TimeValue>>4)&0x0007);
		break;
		case m: 
		  TimeValue=Ds1302_ReceiveByte(0x83); 
		  TimeValue=(TimeValue&0x0f)+10*((TimeValue>>4)&0x0007);
		break;
		case h: 
		  TimeValue=Ds1302_ReceiveByte(0x85); 
		  TimeValue=TimeValue&0x001f;
		break;
		
		case D: 
		  TimeValue=Ds1302_ReceiveByte(0x87); 
		  TimeValue=(TimeValue&0x0f)+10*((TimeValue>>4)&0x0003);
		break;
		case M: 
		  TimeValue=Ds1302_ReceiveByte(0x89); 
		  TimeValue=(TimeValue&0x0f)+10*((TimeValue>>4)&0x0001);
		break;
		case Y: 
		  TimeValue=Ds1302_ReceiveByte(0x8d); 
		  TimeValue=(TimeValue&0x0f)+10*((TimeValue>>4)&0x0f);
		break;
		
	}
	
	return TimeValue;
}





void LcdWriteCom(uint8_t com)	  //写入命令
{
	LCD1602_E = 0;     //使能
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	
	LCD1602_DATAPINS = com;     //放入命令
	delay_ms(1);		//等待数据稳定

	LCD1602_E = 1;	          //写入时序
	delay_ms(5);	  //保持时间
	LCD1602_E = 0;
}

void LcdWriteData(uint8_t dat)			//写入数据
{
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
	delay_ms(1);

	LCD1602_E = 1;   //写入时序
	delay_ms(5);   //保持时间
	LCD1602_E = 0;
}

void LcdInit()						  //LCD初始化子程序
{
 	LcdWriteCom(0x38);  //开显示
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	
	LcdShowChar(1,2,':');
  LcdShowChar(1,5,':');
	LcdShowChar(2,4,'-');
	LcdShowChar(2,7,'-');
	

}

void LcdShowChar(uint8_t y,uint8_t x,uint8_t ch){
if(y==1){
LcdWriteCom(0x80+x);LcdWriteData(ch);
}else{
LcdWriteCom(0xc0+x);LcdWriteData(ch);
}
}

void Lcd_showTime(TimeType T,uint16_t TimeValue){

switch(T){
		
		case s: 
		  LcdShowChar(1,6,'0'+TimeValue/10%10);
			LcdShowChar(1,7,'0'+TimeValue%10);
		  break;
		case m: 
			LcdShowChar(1,3,'0'+TimeValue/10%10);
			LcdShowChar(1,4,'0'+TimeValue%10);
			break;
		case h: 
			LcdShowChar(1,0,'0'+TimeValue/10%10);
			LcdShowChar(1,1,'0'+TimeValue%10);
			break;
		
		case D: 
			LcdShowChar(2,8,'0'+TimeValue/10%10);
			LcdShowChar(2,9,'0'+TimeValue%10);
			break;
		case M: 
			LcdShowChar(2,5,'0'+TimeValue/10%10);
			LcdShowChar(2,6,'0'+TimeValue%10);
			break;
		case Y: 
			LcdShowChar(2,0,'0'+TimeValue/1000%10);
			LcdShowChar(2,1,'0'+TimeValue/100%10);
			LcdShowChar(2,2,'0'+TimeValue/10%10);
			LcdShowChar(2,3,'0'+TimeValue%10);
			break;
		
	}
}

