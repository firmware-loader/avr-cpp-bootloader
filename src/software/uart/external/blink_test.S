.global  blink_test

blink_test:
    sbi 0x4,4
    in  r24,0x5
    ldi r25,lo8(16)
    eor r24,r25
    out 0x5,r24
    ldi r25,0
    ldi r24,0
    ret
