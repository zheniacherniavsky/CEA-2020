.486
.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib libucrt.lib
includelib E:\lib\userlib.lib

ExitProcess PROTO, :DWORD
EXTRN outint : PROC
EXTRN outstr : PROC
EXTRN copystr : PROC

.STACK 4096
.DATA
	     _a	SDWORD	0
	     _b	SDWORD	0
.CONST
	overflow db 'ERROR: VARIABLE OVERFLOW', 0
	null_division db 'ERROR: DIVISION BY ZERO', 0
	     _c1	SDWORD	10
	     _c2	SDWORD	20
	     _c3	SDWORD	0
.CODE

start:
main PROC
	call cea2020
main ENDP

; ---------- - Function definitions--------------------


cea2020 PROC


	; // this is _a expression! int
	push 0
	pop	_a


	; // this is _b expression! int
	push 0
	pop	_b


	; // this is _a expression! int
	push	_c1
	pop	_a


	; // this is _b expression! int
	push	_c2
	pop	_b


	; // this is _b expression! int
	push	_b
	push	_a
	pop eax
	neg eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop	_b

	push	_b
	call	outint ; // at console

	push	_c3	; // this is return of function: main

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