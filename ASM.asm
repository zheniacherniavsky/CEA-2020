.486
.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib libucrt.lib
includelib E:\lib\userlib.lib

ExitProcess PROTO, :DWORD
EXTRN outint : PROC
EXTRN outstr : PROC
EXTRN copystr : PROC
EXTRN strcon : PROC
EXTRN ConvertToChar : PROC

.STACK 4096
.DATA
	     _a	BYTE 255 DUP(0)
	     _b	BYTE 255 DUP(0)
	     _c	BYTE 255 DUP(0)
	     _age	SDWORD	0
	     _sage	BYTE 255 DUP(0)
.CONST
	overflow db 'ERROR: VARIABLE OVERFLOW', 0
	null_division db 'ERROR: DIVISION BY ZERO', 0
	     _c1	BYTE	"this ",0
	     _c2	BYTE	"is ",0
	     _c3	BYTE	"Zhenia!",0
	     _c4	SDWORD	10
	     _c5	SDWORD	123
	     _c6	BYTE	" Age is: ",0
	     _c7	SDWORD	0
.CODE

start:
main PROC
	call cea2020
main ENDP

; ---------- - Function definitions--------------------


cea2020 PROC


	; // this is _a expression! str
	push 	offset _c1
	push	offset _a
	call copystr
	

	; // this is _b expression! str
	push 	offset _c2
	push	offset _b
	call copystr
	

	; // this is _c expression! str
	push 	offset _a
	push 	offset _b
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset _c3
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push	offset _c
	call copystr
	
REPEAT 10
	push 	offset _c
	call	outstr ; // at console

ENDM


	; // this is _age expression! int
	push	_c5
	pop	_age


	; // this is _sage expression! str
	push	_age
	call	ConvertToChar
	push	eax
	push	offset _sage
	call copystr
	

	; // this is _c expression! str
	push 	offset _c
	push 	offset _c6
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset _sage
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push	offset _c
	call copystr
	
	push 	offset _c
	call	outstr ; // at console

	push	_c7	; // this is return of function: main

	jmp EXIT

EXIT_DIV_ON_NULL:
	push offset null_division
	call outstr
	push - 1
	call ExitProcess

EXIT_OVERFLOW:
	push offset overflow
	call outstr
	push - 2
	call ExitProcess

	EXIT:	call ExitProcess
cea2020 ENDP
END start