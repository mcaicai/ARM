
.text
_start:


MAIN:
	BL LED_CONFIG
LOOP:
	BL LED_ON
	BL DELAY
	BL LED_OFF
	BL DELAY
	B LOOP

LED_CONFIG:
	LDR R2,	=0x11000c40
	LDR R1,	=0x10000000
	STR R1,	[R2]
	MOV PC, LR	

LED_OFF:
	LDR R2,	=0x11000c44
	LDR R1,	=0x00000000
	STR R1,	[R2]
	MOV PC, LR

LED_ON:
	LDR R2,	=0x11000c44
	LDR R1,	=0x10000000
	STR R1,	[R2]
	MOV PC, LR

DELAY:
	LDR R1, =1000000
L:
	SUB R1, R1, #1
	CMP R1, #0
	BNE L
	MOV PC, LR

STOP:
	B STOP


.end









