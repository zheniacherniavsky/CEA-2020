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
EXTRN powN : PROC
EXTRN rootN : PROC
EXTRN ConvertToChar : PROC

.STACK 4096
.DATA
	       main_a	SDWORD	0
.CONST
	overflow db 'ERROR: VARIABLE OVERFLOW', 0
	null_division db 'ERROR: DIVISION BY ZERO', 0
	       main_c1	SDWORD	7
	       main_c2	SDWORD	4
	       main_c3	SDWORD	0
.CODE

start:
main PROC
	call cea2020
main ENDP

; ---------- - Function definitions--------------------

cea2020 PROC

	; // this is main_a expression! int
	push	main_c1
	push	main_c2
	pop ebx
	pop eax
	test ebx,ebx
	jz EXIT_DIV_ON_NULL
	cdq
	mov	edx,	0
	idiv ebx
	push edx
	pop	main_a

	push	main_a
	call	outint ; // at console

	push	main_c3	; // this is return of function: main

	jmp EXIT
cea2020 ENDP

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

END start