/* Abe Jordan
 * ECE 477
 * Lab 5
 * Part B
 * Reads avr clock signal and dynamically updates clock
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
int main(int argc, char** argv)
{
		double freq = 0;
		char osccal_add =1;
		char osccal_sign =1;
		char eeprom_[16];
		char eeprom_command[128];
		sprintf(eeprom_write,":020000%X%X%X",
						osccal_sign,
						osccal_add,
			  			(~(osccal_add+osccal_sign+2)+1)&0xFF);
		if (freq > 100.5)

}
