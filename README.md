# ECE 477 Lab 5
Part A simply measures the frequency and gives an updated frequency every second or so.
Part B measures the frequency and attempts to correct it to within 0.5 Hz.
To compile the code run "sudo make all", which should also program the board.
To change the value in eeprom directly from the terminal:

sudo avrdude -C lab5_avrdude_gpio.conf -c pi_1 -p m88p -U eeprom:w:Number1,Number2:m

Where Number1 is the offset (in hex) and Number2 is the sign (0 for negative, 1 for positive)
of the offset.

