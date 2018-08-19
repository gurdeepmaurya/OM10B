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

#ifdef OM10B_EXTRAROUTINES

void OM10B_force_active(unsigned char mode)
{
/*
info=0->Normal mode
info=1->Force active
*/
unsigned char finalconf;

finalconf=OM10B_read(0x12);
if(mode)
	{ finalconf|=0x20;}
else
	{ finalconf&=(~0x20);}

OM10B_write(0x12,finalconf);
}


void OM10B_set_high_res (unsigned char mode)
{
/*
res=0->400DPI
res=1->800DPI
*/ 

unsigned char finalconf;

finalconf=OM10B_read(0x0a);
if(mode)
	{finalconf|=0x10; }
else
	{finalconf&=(~0x10);}

OM10B_write(0x0a,finalconf);
}

void OM10B_disable_hibernation(unsigned char mode, unsigned char mode_type)
{
/*
mode=1->disable hibernate mode :sensor only stays in Standby mode after 1 second on inactivity,
	  or in Active mode if Always active bit is 1
mode=0->Enable Hibernate mode(default):Sensor goes to hibernate mode after "mode_type" seconds

mode_type=0->30seconds(default)
mode_type=1->60seconds
mode_type=2->120seconds
mode_type=3->240seconds
*/

mode&=0x01;
mode_type&=0x03;
unsigned char finalconf;
finalconf=OM10B_read(0x0a);
finalconf&=0xEC;
finalconf|=( (mode<<5)|(mode_type<<2) );
OM10B_write(0x12,finalconf);


}
#endif