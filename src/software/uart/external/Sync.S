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

#define     tmp1     r16
#define     tmp2     r17

#define     TIMEOUT     200

#define     RXPORT      (PORTD - __SFR_OFFSET)
#define     RXPIN       (PIND - __SFR_OFFSET)
#define     RXDDR       (DDRD - __SFR_OFFSET)
#define     RXBIT       0

.global  waitForSyncASM
.global  receiveByte

.comm counter, 2
.comm receiveBuffer, 1

.skipHigh:
        sbic RXPIN,RXBIT
        rjmp .skipHigh
.skipLow:
        sbis RXPIN,RXBIT
        rjmp .skipLow
waitForSyncASM:
        clr bh
        clr bl
.L2:
        sbic RXPIN,RXBIT
        rjmp .L2
.L3:
        adiw bl,5
        sbis RXPIN,RXBIT
        rjmp .L3
.L4:
        sbic RXPIN,RXBIT
        rjmp .L4
.L5:
        sbiw bl,5
        sbis RXPIN,RXBIT
        rjmp .L5
        brmi .skipHigh
.L6:
;        sts counter+1,bh
;        sts counter,bl
;        ldi bh,0
;        ldi bl,0
        ret
receiveByte:
;        clr tmp1
;        clr tmp2
.skipHigh2:
        sbic RXPIN,RXBIT
        rjmp .skipHigh2
        ldi tmp2, lo8(8)
.L9:
        movw xl, bl
        lsr xh
        ror xl
        ;rcall WaitBitcell+1
        ldi tmp2, 9
rxb3:
        rcall WaitBitcell
        lsr tmp1
        sbic RXPIN, RXBIT
        ori tmp1, 128
        dec tmp2
        brne rxb3
;        ldi r25,0
;        ldi r24,0
        ret
WaitBitcell:
        movw xl, bl
wbc0:
        sbiw xl, 4
        brcc wbc0
wbcx:
        sts receiveBuffer,tmp1
;        sts counter+1,bh
;        sts counter,bl
;        clr bh
;        clr bl
;        clr tmp1
        ret
