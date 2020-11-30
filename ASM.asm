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
	        con_c	BYTE 255 DUP(0)
	       main_a	BYTE 255 DUP(0)
	       main_b	BYTE 255 DUP(0)
	       main_c	BYTE 255 DUP(0)
	       main_d1	SDWORD	0
	       main_d2	SDWORD	0
	       main_d3	SDWORD	0
	       main_j	BYTE 255 DUP(0)
	       main_age	SDWORD	0
	       main_sage	BYTE 255 DUP(0)
.CONST
	overflow db 'ERROR: VARIABLE OVERFLOW', 0
	null_division db 'ERROR: DIVISION BY ZERO', 0
	       main_c1	BYTE	"Zheka",0
	       main_c2	BYTE	"Sabina",0
	       main_c3	SDWORD	2
	       main_c4	SDWORD	7
	       main_c5	BYTE	"this ",0
	       main_c6	BYTE	"is ",0
	       main_c7	BYTE	"Zhenia!",0
	       main_c8	SDWORD	10
	       main_c9	SDWORD	123
	       main_c10	BYTE	" Age is: ",0
	       main_c11	SDWORD	0
.CODE

start:
main PROC
	call cea2020
main ENDP

; ---------- - Function definitions--------------------


con PROC con_a :DWORD, con_b :DWORD


	; // this is con_c expression! str
	mov	eax, con_a
	push	eax
	mov	eax, con_b
	push	eax
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push	offset con_c
	call copystr
	
	mov eax, offset con_c
	push	eax
	ret	; // this is return of function: con
con ENDP

lol PROC lol_a :SDWORD, lol_b :SDWORD


	; // this is lol_a expression! int
	push	lol_a
	push	lol_b
	pop	eax
	pop	ebx
	add	eax,	ebx
	push	eax
	pop	lol_a

	push lol_a
	ret	; // this is return of function: lol
lol ENDP



cea2020 PROC


	; // this is main_a expression! str
	push 	offset main_c1
	push	offset main_a
	call copystr
	

	; // this is main_b expression! str
	push 	offset main_c2
	push	offset main_b
	call copystr
	

	; // this is main_c expression! str
	push 	offset main_b
	push 	offset main_a
	call	con
	push	eax
	push	offset main_c
	call copystr
	

	; // this is main_d1 expression! int
	push	main_c3
	pop	main_d1


	; // this is main_d2 expression! int
	push	main_c4
	pop	main_d2


	; // this is main_d3 expression! int
	push	main_d2
	push	main_d1
	call	lol
	push	eax
	pop	main_d3

	push	main_d3
	call	outint ; // at console

	push 	offset main_c
	call	outstr ; // at console


	; // this is main_a expression! str
	push 	offset main_c5
	push	offset main_a
	call copystr
	

	; // this is main_b expression! str
	push 	offset main_c6
	push	offset main_b
	call copystr
	

	; // this is main_j expression! str
	push 	offset main_a
	push 	offset main_b
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset main_c7
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push	offset main_j
	call copystr
	
REPEAT 10
	push 	offset main_j
	call	outstr ; // at console

ENDM


	; // this is main_age expression! int
	push	main_c9
	pop	main_age


	; // this is main_sage expression! str
	push	main_age
	call	ConvertToChar
	push	eax
	push	offset main_sage
	call copystr
	

	; // this is main_j expression! str
	push 	offset main_j
	push 	offset main_c10
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset main_sage
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push	offset main_j
	call copystr
	
	push 	offset main_j
	call	outstr ; // at console

	push	main_c11	; // this is return of function: main

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