#include <avr/io.h>
#define     bl       r24
#define     bh       r25

#define     TIMEOUT     200

#define     RXPORT      (PORTD - __SFR_OFFSET)
#define     RXPIN       (PIND - __SFR_OFFSET)
#define     RXDDR       (DDRD - __SFR_OFFSET)
#define     RXBIT       0

.global  waitForSyncASM

.comm counter, 2

waitForSyncASM:
.L2:
        sbic RXPIN, RXBIT
        rjmp .L2
        ldi r25,0
        ldi r24,0
.L4:
        sbic RXPIN, RXBIT
        rjmp .L3
        adiw r24,1
        rjmp .L4
.L3:
        sbic RXPIN, RXBIT
        rjmp .L3
.L6:
        sbic RXPIN, RXBIT
        rjmp .L5
        sbiw r24,1
        rjmp .L6
.L5:
        sbrs r25,7
        rjmp .L7
.L8:
        sbic RXPIN, RXBIT
        rjmp .L8
.L9:
        sbis RXPIN, RXBIT
        rjmp .L9
        rjmp .L2
.L7:
        sts counter+1,r25
        sts counter,r24
        ldi r25,0
        ldi r24,0
        ret