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
	     _name1	BYTE 255 DUP(0)
	     _age	SDWORD	0
	     _name2	BYTE 255 DUP(0)
.CONST
	overflow db 'ERROR: VARIABLE OVERFLOW', 0
	null_division db 'ERROR: DIVISION BY ZERO', 0
	     _c1	BYTE	"Zheka",0
	     _c2	SDWORD	18
	     _c3	SDWORD	1
	     _c4	BYTE	"NAME:",0
	     _c5	BYTE	"AGE:",0
	     _c6	SDWORD	0
	     _c7	SDWORD	0
.CODE

start:
main PROC
	call cea2020
main ENDP

; ---------- - Function definitions--------------------


cea2020 PROC


	; // this is _name1 expression! str
	push 	offset _c1
	push	offset _name1
	call copystr
	

	; // this is _age expression! int
	push 0
	pop	_age

REPEAT 18

	; // this is _age expression! int
	push	_age
	push	_c3
	pop	eax
	pop	ebx
	add	eax,	ebx
	push	eax
	pop	_age

ENDM

	push 	offset _c4
	call	outstr ; // at console

	push 	offset _name1
	call	outstr ; // at console

	push 	offset _c5
	call	outstr ; // at console

	push	_age
	call	outint ; // at console


	; // this is _age expression! int
	push	_age
	push	_c6
	pop ebx
	pop eax
	test ebx,ebx
	jz EXIT_DIV_ON_NULL
	cdq
	idiv ebx
	push eax
	pop	_age

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