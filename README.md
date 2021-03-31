# ece477-lab5

# program the fuses
sudo avrdude -C ~/avrdude_gpio.conf -c pi_1 -p m88p -U lfuse:w:0xa2:m -U hfuse:w:0xdf:m -U efuse:w:0xf9:m

# create hex files and flash ATMEGA88PA
# lab5a
avr-gcc -mmcu=atmega88pa lab5a.c -o lab5a
avr-objcopy -j .text -j .data -O ihex lab5a lab5a.hex
sudo avrdude -C ./lab5_avrdude_gpio.conf -c pi_1 -p m88p -U flash:w:lab5a.hex:i

# lab5b
avr-gcc -mmcu=atmega88pa lab5b.c -o lab5b
avr-objcopy -j .text -j .data -O ihex lab5b lab5b.hex
sudo avrdude -C ./lab5_avrdude_gpio.conf -c pi_1 -p m88p -U flash:w:lab5b.hex:i
