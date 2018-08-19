/*
Copyright (C) 2014  Akshit Maurya

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <avr/io.h>
#include <util/delay.h>

#include "OM10B.h"

#ifdef OM10B_EXTRAROUTINES
#include <OM10B_extraroutines.c>
#endif

#define _CONCAT(a,b) a##b
 #define PORT(x) _CONCAT(PORT,x)
 #define PIN(x) _CONCAT(PIN,x)
 #define DDR(x) _CONCAT(DDR,x)


#define SCLK_PORT PORT(SCLK_IO_PORT)
#define SCLK_DDR DDR(SCLK_IO_PORT)

#define SDA_PORT PORT(SDA_IO_PORT)
#define SDA_DDR DDR(SDA_IO_PORT)
#define SDA_PIN PIN(SDA_IO_PORT)

#define PD_PORT PORT(PD_IO_PORT)
#define PD_DDR DDR(PD_IO_PORT)

#define SCLK_LOW SCLK_PORT&=(~(1<<SCLK_IO_PIN))
#define SCLK_HIGH SCLK_PORT|=(1<<SCLK_IO_PIN)

#define SDA_LOW SDA_PORT&=(~(1<<SDA_IO_PIN))
#define SDA_HIGH SDA_PORT|=(1<<SDA_IO_PIN)

#define PD_LOW PD_PORT&=(~(1<<PD_IO_PIN))
#define PD_HIGH PD_PORT|=(1<<PD_IO_PIN)

#if (F_CPU<8000000)
#define NS_WAIT 1

#elif  (F_CPU==8000000)
#define NS_WAIT 2

#else
#define NS_WAIT 3

#endif



void _delay_ns()
{
uint8_t count=NS_WAIT;
while(count--)
{}

}

unsigned char OM10B_read(unsigned char s_addr){
unsigned char i,s_data=0;
	
	s_addr&=0x7F; //ensure MSB is 0 for read operation;

//write address to bus loop
for(i=0;i<8;i++){
		SCLK_LOW; //PORTB&=~(0x01); //sclk = 0;
			
			if(s_addr & 0x80)
				{ SDA_HIGH;  } //sda = 1;
			else
			{  SDA_LOW;  }  // sda = 0;

				_delay_ns();// 150ns delay approx;

		SCLK_HIGH; // sclk = 1;

				_delay_ns();//150ns delay approx;

		s_addr<<= 1;
		
				}
		
		//change I/O configuration sda pin as input
		SDA_DDR&= (~(1<<SDA_IO_PIN)); //Loading DDRB register to make SDA pin as input
		SDA_PORT&= (~(1<<SDA_IO_PIN)); //resetting sda pin
		//wait for atleast 100us as per specs
		_delay_us(150);
//reading data from bus loop starts
for(i=0;i<8;i++){
		SCLK_LOW; //sclk = 0;

				_delay_ns();//150ns delay approx;

		SCLK_HIGH; //sclk = 1;

				_delay_ns();//150ns delay approx;
				s_data<<= 1; //shift data bits for this cycle

				if(SDA_PIN & (1<<SDA_IO_PIN) )
				{ s_data|=0x01; } //sda=1;
			else
			{  s_data&=0xFE;  } //sda = 0;

		
		
				}

		//change I/O configuration sda pin as output
		SDA_DDR|=(1<<SDA_IO_PIN);//Restoring DDRB register state;
		_delay_us(1);
return s_data;
		
		}
		



void OM10B_power_down(unsigned int k)
{

 //resetting sensor
	  PD_HIGH; //PD=1;
	 _delay_ms(1);//> 300us delay

	PD_LOW; //PD=0;
	if(k<400) //delay should be atleast 300ms according to specs
	{k=400;}
	_delay_ms(k); //rise up sensor delay 

}


void OM10B_init() {
SCLK_DDR|=(1<<SCLK_IO_PIN); //configuring SCLK as output
SDA_DDR|=(1<<SDA_IO_PIN); //configuring SDA as output
PD_DDR|=(1<<PD_IO_PIN); //configuring PD as output
SCLK_HIGH;  //clk high
OM10B_power_down(400); //performing powerdown
OM10B_reset(); 

}

void OM10B_write(unsigned char s_addr,unsigned char s_data){
unsigned char i;
unsigned int payload;
	s_addr|=0x80; //ensure MSB is 1 for read operation;

payload=s_addr;
payload<<=8;
payload|=s_data;
//write address to bus loop
for(i=0;i<16;i++){
		SCLK_LOW; //PORTB&=~(0x01); //sclk = 0;
			
			if(payload & 0x8000)
				{ SDA_HIGH;  } //sda = 1;
			else
			{  SDA_LOW;  }  //sda = 0;

				_delay_us(1);

		SCLK_HIGH;  //sclk = 1;

				_delay_us(1);

		payload<<= 1;
		
				}

_delay_us(150); //100 microseconds delay required according to specs

}

void OM10B_reset()
{
OM10B_write(0x0a,0x80); //RESET bit set at 0x0A
OM10B_write(0x0a,0x00); //RESET bit cleared at 0x0A
}

