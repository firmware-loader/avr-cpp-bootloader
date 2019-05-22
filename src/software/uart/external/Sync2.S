;original code:
;static auto waitForSync() {
;     auto tmp = 0;
;     while (true) {
;         tmp = 0;
;         while (isHigh()) {}           //skip first high
;
;         while (!isHigh()) {         //measure first low time
;             tmp++;
;         }
;         tmp++;
;
;         while (isHigh()) {}         //wait for 2nd low
;
;         while (!isHigh()) {
;             tmp--;
;         }
;         tmp--;
;
;         if (tmp < 0) {
;             while (isHigh()) {}
;             while (!isHigh()) {}      //repeat until in sync
;         } else {
;             break;                  //sync
;         }
;     }
;     counter = tmp;
; }


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

.L7:
        sbic RXPIN,RXBIT
        rjmp .L7
.L8:
        sbis RXPIN,RXBIT
        rjmp .L8
        rjmp .L2
waitForSyncASM:
.L2:
        sbic RXPIN,RXBIT
        rjmp .L2
        ldi r25,0
        ldi r24,0
.L3:
        adiw r24,1
        sbis RXPIN,RXBIT
        rjmp .L3
.L4:
        sbic RXPIN,RXBIT
        rjmp .L4
.L5:
        sbiw r24,1
        sbis RXPIN,RXBIT
        rjmp .L5
        brmi .L7
.L6:
        sts counter+1,r25
        sts counter,r24
        ldi r25,0
        ldi r24,0
        ret