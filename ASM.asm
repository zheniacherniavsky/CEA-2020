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
EXTRN _AND : PROC
EXTRN _OR : PROC
EXTRN _NOT : PROC
EXTRN tostr : PROC

.STACK 4096
.DATA
	       head_b	BYTE 255 DUP(0)
	       main_menu1	BYTE 255 DUP(0)
	       main_tomat	BYTE 255 DUP(0)
	       main_iToma	SDWORD	0
	       main_sToma	BYTE 255 DUP(0)
	       main_potat	BYTE 255 DUP(0)
	       main_iPota	SDWORD	0
	       main_sPota	BYTE 255 DUP(0)
.CONST
	overflow db 'ERROR: VARIABLE OVERFLOW', 0
	null_division db 'ERROR: DIVISION BY ZERO', 0
	       head_c1	BYTE	"+-+-+-+-+-+ ",0
	       head_c2	BYTE	" +-+-+-+-+-+",0
	         f1_c3	SDWORD	10
	         f2_c4	SDWORD	2
	      funcs_c5	SDWORD	0
	       main_c6	BYTE	"Завтрак",0
	       main_c7	BYTE	"Помидор",0
	       main_c8	SDWORD	32
	       main_c9	BYTE	" ",0
	       main_c10	BYTE	" шт.",0
	       main_c11	BYTE	"Картофель",0
	       main_c12	SDWORD	3
	       main_c13	BYTE	" ",0
	       main_c14	BYTE	" шт.",0
	       main_c15	SDWORD	0
.CODE

start:
main PROC
	call cea2020
main ENDP

; ---------- - Function definitions--------------------


head PROC head_a :DWORD


	; // this is head_b expression! str
	push 	offset head_c1
	push	[head_a]
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset head_c2
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push	offset head_b
	call copystr
	
	push 	offset head_b
	call	outstr ; // at console

	mov eax, offset head_b
	push	eax
	ret	; // this is return of function: head
head ENDP

f1 PROC f1_num :SDWORD


	; // this is f1_num expression! int
	push	f1_num
	push	f1_c3
	pop ebx
	pop eax
	test ebx,ebx
	jz EXIT_DIV_ON_NULL
	cdq
	mov	edx,	0
	idiv ebx
	push edx
	pop	f1_num

	push f1_num
	ret	; // this is return of function: f1
f1 ENDP

f2 PROC f2_num :SDWORD


	; // this is f2_num expression! int
	push	f2_c4
	push	f2_num
	call	powN
	push	eax
	pop	f2_num

	push f2_num
	ret	; // this is return of function: f2
f2 ENDP

funcs PROC 

	push funcs_c5
	ret	; // this is return of function: funcs
funcs ENDP



cea2020 PROC


	; // this is main_menu1 expression! str
	push 	offset main_c6
	push	offset main_menu1
	call copystr
	

	; // this is main_menu1 expression! str
	push 	offset main_menu1
	call	head
	push	eax
	push	offset main_menu1
	call copystr
	

	; // this is main_tomat expression! str
	push 	offset main_c7
	push	offset main_tomat
	call copystr
	

	; // this is main_iToma expression! int
	push	main_c8
	call	f1
	push	eax
	pop	main_iToma


	; // this is main_sToma expression! str
	push	main_iToma
	call	tostr
	push	eax
	push	offset main_sToma
	call copystr
	

	; // this is main_tomat expression! str
	push 	offset main_tomat
	push 	offset main_c9
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset main_sToma
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset main_c10
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push	offset main_tomat
	call copystr
	
	push 	offset main_tomat
	call	outstr ; // at console


	; // this is main_potat expression! str
	push 	offset main_c11
	push	offset main_potat
	call copystr
	

	; // this is main_iPota expression! int
	push	main_c12
	call	f2
	push	eax
	pop	main_iPota


	; // this is main_sPota expression! str
	push	main_iPota
	call	tostr
	push	eax
	push	offset main_sPota
	call copystr
	

	; // this is main_potat expression! str
	push 	offset main_potat
	push 	offset main_c13
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset main_sPota
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push 	offset main_c14
	call strcon
	jo EXIT_OVERFLOW
	push	eax
	push	offset main_potat
	call copystr
	
	push 	offset main_potat
	call	outstr ; // at console

	push	main_c15	; // this is return of function: main

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