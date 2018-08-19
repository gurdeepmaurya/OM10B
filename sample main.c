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
//////////////////////////////Code Starts///////////////////////////////////

#include<avr/io.h> //for general IO
#include<utils/delay.h> //has delay function prototypes.

#include "OM10B.h" //for adding library support.

void main()
{
unsigned char temp=0; //just a temporay variable.
char x_delta=0,y_delta=0; 
//these variables would be used to store x and y displacement respectively.
/* 
It's important to note that values of displacement data at registers 0x03 and 0x04 of OM10B IC are stored in signed representation ,
Whereas our function unsigned char OM10B_read(s_addr) returns unsigned char value,
it is coder's responsibility to cast it to signed char type,
this is the reason why we are using data type of variable x_delta and y_delta as char and not unsigned char.
*/

while(1){
 	temp=OM10B_read(0x02);     //reading data from 0x02 location
	if(temp&0x80) //if MOT bit is set
     		{
        	 x_delta=OM10B_read(0x03);    //reading x displacement
        	 y_delta=OM10B_read(0x04);    //reading y displacement
       		}
	}

}

/////////////////////////////Code Ends////////////////////////////////////
