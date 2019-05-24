;original code:
;static auto waitForSyncASM() {
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

#define     tmp1     r22
#define     tmp2     r23

#define     TIMEOUT     200

#define     RXPORT      (PORTD - __SFR_OFFSET)
#define     RXPIN       (PIND - __SFR_OFFSET)
#define     RXDDR       (DDRD - __SFR_OFFSET)
#define     RXBIT       0

.global  asm_uart_waitForSyncASM
.global  asm_uart_receiveByte

.comm asm_uart_receiveBuffer, 1

asm_uart_skipHigh:
        sbic RXPIN,RXBIT
        rjmp asm_uart_skipHigh
asm_uart_skipLow:
        sbis RXPIN,RXBIT
        rjmp asm_uart_skipLow
asm_uart_waitForSyncASM:
        clr bh
        clr bl
asm_uart_L2:
        sbic RXPIN,RXBIT
        rjmp asm_uart_L2
asm_uart_L3:
        adiw bl,5
        sbis RXPIN,RXBIT
        rjmp asm_uart_L3
asm_uart_L4:
        sbic RXPIN,RXBIT
        rjmp asm_uart_L4
asm_uart_L5:
        sbiw bl,5
        sbis RXPIN,RXBIT
        rjmp asm_uart_L5
        brmi asm_uart_skipHigh
asm_uart_wop0:
        rcall asm_uart_receiveByte
        lds tmp1, asm_uart_receiveBuffer
        cpi tmp1, lo8(85)
        brne asm_uart_wop0
        ret
asm_uart_receiveByte:
        sbic RXPIN,RXBIT
        rjmp asm_uart_receiveByte
;        ldi tmp2, lo8(8)
asm_uart_L9:
        movw xl, bl
        lsr xh
        ror xl
        ldi tmp2, 9
asm_uart_rxb3:
        rcall asm_uart_WaitBitcell
        lsr tmp1
        sbic RXPIN, RXBIT
        ori tmp1, 128
        dec tmp2
        brne asm_uart_rxb3
        ret
asm_uart_WaitBitcell:
        movw xl, bl
asm_uart_wbc0:
        sbiw xl, 4
        brcc asm_uart_wbc0
asm_uart_wbcx:
        sts asm_uart_receiveBuffer,tmp1
        ret
