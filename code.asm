.486
.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib userlib.lib

ExitProcess PROTO, :DWORD
outint PROTO, :DWORD ; 1 arg [int] // out int on console

.STACK 4096
.DATA
	examp	DD	0
	    b	DD	0
	danik	DD	0
	 div1	DD	0
	 div2	DD	0
	 div3	DD	0
	 div4	DD	0
.CONST
	   c1	DD	3
	   c2	DD	9
	   c3	DD	4
	   c4	DD	8
	   c5	DD	7
	   c6	DD	50
	   c7	DD	85
	   c8	DD	74
	   c9	DD	59
	  c10	DD	94
	  c11	DD	8
	  c12	DD	2
	  c13	DD	20
	  c14	DD	3
	  c15	DD	3
	  c16	DD	9
	  c17	DD	3
	  c18	DD	4
	  c19	DD	0
.CODE
; ---------- - Function definitions--------------------


cea2020 PROC


	; // this is examp expression!
	push	c1
	push	c2
	pop	eax
	pop	ebx
	add	eax,	ebx
	push	eax
	pop	examp


	; // this is b expression!
	push	c3
	push	c4
	push	c5
	pop	eax
	pop	ebx
	imul	ebx
	push	eax
	pop	eax
	pop	ebx
	add	eax,	ebx
	push	eax
	pop	b


	; // this is danik expression!
	push	c6
	push	c7
	push	c8
	push	c9
	pop	eax
	pop	ebx
	add	eax,	ebx
	push	eax
	pop	eax
	pop	ebx
	imul	ebx
	push	eax
	push	c10
	pop	eax
	pop	ebx
	imul	ebx
	push	eax
	pop	eax
	pop	ebx
	add	eax,	ebx
	push	eax
	pop	danik


	; // this is div1 expression!
	push	c11
	push	c12
	pop ebx
	pop eax
	test ebx,ebx
	jz EXIT_DIV_ON_NULL
	cdq
	idiv ebx
	push eax
	pop	div1


	; // this is div2 expression!
	push	c13
	push	c14
	pop ebx
	pop eax
	test ebx,ebx
	jz EXIT_DIV_ON_NULL
	cdq
	idiv ebx
	push eax
	pop	div2


	; // this is div3 expression!
	push	c15
	push	c16
	pop ebx
	pop eax
	test ebx,ebx
	jz EXIT_DIV_ON_NULL
	cdq
	idiv ebx
	push eax
	pop	div3


	; // this is div4 expression!
	push	c17
	push	c18
	pop ebx
	pop eax
	test ebx,ebx
	jz EXIT_DIV_ON_NULL
	cdq
	idiv ebx
	push eax
	pop	div4

	push	c19	; // this is return of function: main
	call ExitProcess

EXIT_DIV_ON_NULL:
; // here is console output with error
cea2020 ENDP

start:
	call cea2020
end start