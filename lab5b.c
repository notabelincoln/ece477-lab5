/* Abe Jordan
 * ECE 477
 * Lab 5
 * Part A
 * Reads avr clock signal and determines if too fast or slow
 */
#define PRINT_DELAY_MAX 9999999
#define TOLERANCE 0.5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "wiringPi.h"
int main(int argc, char** argv)
{
		double frequencies[10]; // array of frequencies
		/* average frequency, sum of frequencies in array, minimum frequency
		 * within tolerance, and maximum frequency within tolerance */
		double average_frequency, sum_frequencies, fmin, fmax;
		unsigned char i;
		unsigned int print_delay;
		char eeprom_write[256];
		char gpio_previous, gpio_current; // Previous and current reading GPIO state
		char osccal_sign, osccal_val; // Sign and value for osccal
		clock_t clock0, clockf; // initial and final clock cycles of rising edge

		// Parameters for running avrdude via system
		char avr_config[64] = "./lab5_avrdude_gpio.conf"; // config file to use
		char avr_programmer[8] = "pi_1"; // programmer to use
		char avr_chip[16] = "m88p"; // type of avr chip
		char avr_section[16] = "eeprom"; // access eeprom of chip
		char avr_op = 'w'; // write to eeprom
		char op_type = 'm'; // immediatle values

		// Set up values for variables
		memset(frequencies,0,10*sizeof(double));
		memset(eeprom_write,0,128*sizeof(char));
		osccal_sign = 1;
		osccal_val = 0;
		average_frequency = 0;
		sum_frequencies = 0;
		print_delay = 0;
		clock0 = 0;
		fmin = 100.0 - TOLERANCE;
		fmax = 100.0 + TOLERANCE;

		// Set up GPIO 6 as input for reading signal
		wiringPiSetup();
		pinMode(6, INPUT);
		pullUpDnControl(6, PUD_UP);
		gpio_previous = digitalRead(6);

		while(1) {
				// Add frequencie to list
				gpio_current = digitalRead(6);
				if ((gpio_current == 1) && (gpio_previous == 0)) {
						for (i = 0; i < 9; i++)
								frequencies[i] = frequencies[i+1];
						
						// Calculate frequency of output waveform
						clockf = clock();
						frequencies[9] = (double)(CLOCKS_PER_SEC)/(clockf - clock0);
						clock0 = clockf;
				}


				print_delay = (print_delay == PRINT_DELAY_MAX) ? 0 : (print_delay + 1);
				gpio_previous = gpio_current;
				
				// Compute average frequency
				sum_frequencies = 0;
				for (i = 0; i < 10; i++)
						sum_frequencies += frequencies[i];

				average_frequency = sum_frequencies/10;

				if (print_delay == PRINT_DELAY_MAX) {
						/*for (i = 0; i < 10; i++) {
								printf("frequencies[i] = %lf\n",frequencies[i]);
								fflush(stdout);
						}*/

						printf("Frequency: ");
						fflush(stdout);
						// Check if frequency is some ridiculous number indicating an error
						if (average_frequency > 8000000) {
								printf("INVALID\n");
								fflush(stdout);
								osccal_val = 0;
								// Check if frequency is above tolerance
						} else if (average_frequency > fmax) {
								printf("%8.3lf Hz, too high, adjusting eeprom\n",
												average_frequency);
								fflush(stdout);
								if (osccal_sign == 0)
										osccal_val = 1;
								else
										osccal_val++;
								osccal_sign = 1;
						// Check if frequency is below tolerance
						} else if (average_frequency < fmin) {
								printf("%8.3lf Hz, too low, adjusting eeprom\n",
												average_frequency);
								fflush(stdout);
								if (osccal_sign == 1)
										osccal_val = 0;
								else
										osccal_val++;
								osccal_sign = 0;
						// Frequency is within bounds
						} else {
								printf("%8.3lf Hz, just right\n",
												average_frequency);
						}

						if ((average_frequency > fmax) || (average_frequency < fmin)) {
							sprintf(eeprom_write,
										"sudo avrdude -C %s -c %s -p %s -U %s:%c:%#x,%#x:%c",
										avr_config,
										avr_programmer,
										avr_chip,
										avr_section,
										avr_op,
										osccal_val,
										osccal_sign,
										op_type);
							//printf("%s\n",eeprom_write);
							//fflush(stdout);
							system(eeprom_write);
						}
				}
		}
}

