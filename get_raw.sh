#!/bin/bash
/bin/gcc9/linux/bin/avr-g++ -Os -mmcu=atmega328p -DF_CPU=12000000UL -E main.cpp > "debug.txt"
