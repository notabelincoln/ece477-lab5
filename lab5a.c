/* Abe Jordan
 * ECE 477
 * Lab 5
 * Part A
 * Reads avr clock signal and determines if too fast or slow
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "wiringPi.h"
int main(int argc, char** argv)
{
		double frequencies[10]; // array of frequencies
		double average_frequency; // average of frequencies
		double sum_frequencies; // sum of all frequencies
		unsigned char i;
		unsigned int print_delay;
		char gpio_previous; // Previous reading GPIO state
		char gpio_current; // Current reading GPIO state
		clock_t clock0, clockf; // initial and final clock cycles of rising edge

		// Set up values for variables
		memset(frequencies,100,10*sizeof(double));
		average_frequency = 100;
		sum_frequencies = 100 * 10;
		print_delay = 0;
		clock0 = 0;

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
						clockf = clock();
						frequencies[9] = (double)(CLOCKS_PER_SEC)/(clockf - clock0);
						clock0 = clockf;
				}
				gpio_previous = gpio_current;

				// Compute average frequency
				sum_frequencies = 0;
				for (i = 0; i < 10; i++)
						sum_frequencies += frequencies[i];
				average_frequency = sum_frequencies/10;

				if (print_delay == 999999) {
						// Check bounds of frequency within tolerance
						if (average_frequency > 8000000)
								printf("Frequency out of bounds\n");
						else if (average_frequency > 100.5) {
								printf("Frequency is about %lf Hz, too high\n",
												average_frequency);
						} else if (average_frequency < 99.5) {
								printf("Frequency is about %lf Hz, too low\n",
												average_frequency);
						} else {
								printf("Frequency is about %lf Hz, just right\n",
												average_frequency);
						}
				}
				print_delay = (print_delay == 999999) ? 0 : (print_delay + 1);
		}
}

