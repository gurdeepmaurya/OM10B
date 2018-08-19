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


#include<avr/io.h>
#include<util/delay.h>

//#define OM10B_EXTRAROUTINES //Define this incase you want Supplementary routines

#define ACTIVATE 1
#define DEACTIVATE 0

#define SCLK_IO_PORT B   //PORT where SCLK of OM10B Pin is connected to Atmega
#define SCLK_IO_PIN PB0 //pin where SCLK of OM10B Pin is connected to Atmega

#define SDA_IO_PORT B    //PORT where SDA of OM10B Pin is connected to Atmega
#define SDA_IO_PIN  PB1 //pin where SDA of OM10B Pin is connected to Atmega

#define PD_IO_PORT B  	   //PORT where PD of OM10B Pin is connected to Atmega
#define PD_IO_PIN	 PB2  //pin where PD of OM10B Pin is connected to Atmega

unsigned char OM10B_read(unsigned char s_addr);
/*
This function is used to read data from particular address,s_addr,and returns 8-bit data that has been retreived from it.
It's important to note that values of displacement data at registers 0x03 and 0x04 of OM10B IC are stored in signed 8-bit representation,
Whereas our function "unsigned char OM10B_read(s_addr)" returns unsigned char value,it is coder's responsibility to cast it to signed char type.
*/

void OM10B_write(unsigned char s_addr,unsigned char s_data);
/*
This function is used to write data,s_data, to address, s_addr.
*/

void OM10B_init();
/*
This function is used to initialize OM10B library.
It neither accepts  nor  returns anything.
*/

void OM10B_reset();
/*
This function resets OM10B sensor.It neither accepts  nor  returns anything.
*/

void OM10B_power_down(unsigned int k);
/*
This function is used if incase user wants to enforce power down mode for k milliseconds.
Power down mode is useful if you want to reduce power consumption;
LED will shut down for k milliseconds and also IC will only draw 0.02milliampere current.
*/


#ifdef OM10B_EXTRAROUTINES
void OM10B_set_high_res (unsigned char mode);
/*
This Function is used to configure FORCE ACTIVE feature of OM10B sensor.
It accepts an unsigned char argument, mode,Which when '1',activates FORCE ACTIVE mode and when '0' deactivates FORCE ACTIVE mode,i.e. DEFAULT NORMAL mode.
In DEFAULT NORMAL mode sensor will go to STANDBY mode after 1 sec of inactivity and to HIBERNATION mode(from STANBYMODE) after 30 seconds of inactivity(depending upon bit 2 and bit 3 of configuration register 0x12,
this time interval can be increased to 60/120/240seconds ).This function returns nothing.
OM10B_force_active(1);    //FORCE ACTIVE mode activated
OM10B_force_active(0);    //FORCE ACTIVE mode de-activated

or you can also use

OM10B_force_active(ACTIVATE);    //FORCE ACTIVE mode activated
OM10B_force_active(DEACTIVATE);    //FORCE ACTIVE mode de-activated

Note same thing can be achieved by explicitly performing a write operation at address 0x12 with data ,0x20, which will result in setting up of bit 5 ;as in
OM10B_write(0x12,0x20);  //Write 0x20 i.e. 0b00100000 at address 0x12
*/

void OM10B_force_active(unsigned char mode);
/*
This function is used to activate 800DPI high resolution mode.By default OM10B works at 400DPI,
but if someone wants more senstivity,just use this function.
*/

void OM10B_disable_hibernation(unsigned char mode, unsigned char mode_type)
/*
Our sensor changes it's state from ACTIVE to STANDBY to HIBERNATE
ACTIVE to STANDBY Transition occurs after 1 second of inactivity provided FORCE ACTIVE feature is disabled in which case sensor stays in ACTIVE state always
STANDBY to HIBERNATE transition occurs after 30 seconds (or 60/120/240 seconds depending upon value of mode_type argument).
This Function accepts 2 arguments,first is mode,which when 1 disables HIBERNATION state,when 0 enables hibernation state(default),
Second argument mode_type determines after how many seconds of non-movement will our sensor go to HIBERNATION state,if Hibernation mode has been activated.
mode_type= 0 means time interval is 30 seconds
mode_type= 1 means time interval is 60 seconds
mode_type= 2 means time interval is 120 seconds
mode_type= 3 means time interval is 240 seconds

Thus
OM10B_disable_hibernation(0,1);  
//Activates Hibernation Mode and sets transition time interval to 60 Seconds.
*/

#endif


