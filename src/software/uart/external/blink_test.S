#include <avr/io.h>

temp = 16
.comm counter, 1

.global  blink_test

.equ    RXPORT, (PORTB - __SFR_OFFSET)
.equ    RXPIN, (PINB - __SFR_OFFSET)
.equ    RXDDR, (DDRB - __SFR_OFFSET)

blink_test:
    ;push temp                      ; wichtig: Benutzte Register und das
    in temp,_SFR_IO_ADDR(SREG)
    push temp

    lds temp,counter
    inc temp

    sbi RXDDR,4
    in r24,RXPORT
    ldi r25,lo8(16)
    eor r24,r25
    out RXPORT,r24
    ldi r25,0
    ldi r24,0

    sts counter,temp

    pop temp                       ; die benutzten Register wiederherstellen
    out _SFR_IO_ADDR(SREG),temp
    ret
