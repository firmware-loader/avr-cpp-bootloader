#include <avr/io.h>
#define     bl       r24
#define     bh       r25

#define     TIMEOUT     200

#define     RXPORT      (PORTD - __SFR_OFFSET)
#define     RXPIN       (PIND - __SFR_OFFSET)
#define     RXDDR       (DDRD - __SFR_OFFSET)
#define     RXBIT       0

.global  SyncReceiveDecryptChainDetectMilestone

.comm counter, 2

SyncReceiveDecryptChainDetectMilestone:
;
                rcall TimeOutStopState  ; wait for startbit
;
; synchronise & autobaud to either [0-00111001-1] ($9C) or [0-00110011-1] ($CC)
;
PreSyncAutobaud:
psa00:
                sbic RXPIN, RXBIT       ; wait out any first 1-state
                rjmp psa00
psa01:
                clr bl                  ; clear
                clr bh                  ; baudrate word
                rcall TimeOutStopState  ; wait for startbit
psa10:
                adiw bl, 2              ; add 1-state
                sbic RXPIN, RXBIT
                rjmp psa10
psa11:
                rcall TimeOutStopState  ; wait for next startbit
psa20:
                sbiw bl, 2              ; subtract 1-state
                sbic RXPIN, RXBIT       ; to get an error compensated value
                rjmp psa20

                brmi SyncReceiveDecryptChainDetectMilestone
                sts (counter + 1), bh
                sts (counter + 0), bl
                ret
TimeOutStopState:
toss0:
                wdr                     ; apply periodic WD Reset in outer loop
toss1:                                  ; consider normal/inverted RX-logic
                sbic RXPIN, RXBIT       ; if RX=0 occurs (normal startbit)
                ret