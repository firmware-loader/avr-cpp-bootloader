#include <avr/io.h>

temp = 16
.comm counter, 1

.global  blink_test

.equ    RXPORT, PORTB
.equ    RXPIN, PINB
.equ    RXDDR, DDRB

blink_test:
    ;push temp                      ; wichtig: Benutzte Register und das
    in temp,_SFR_IO_ADDR(SREG)
    push temp

    lds temp,counter
    inc temp

    sbi 0x4,4
    in r24,0x5
    ldi r25,lo8(16)
    eor r24,r25
    out 0x5,r24
    ldi r25,0
    ldi r24,0

    sts counter,temp

    pop temp                       ; die benutzten Register wiederherstellen
    out _SFR_IO_ADDR(SREG),temp
    ret
