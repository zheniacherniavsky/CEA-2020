.686
.MODEL FLAT, STDCALL
includelib kernel32.lib
ExitProcess PROTO, :DWORD
; // ----------- EXTRN functions declarations -----------



.STACK 4096
.DATA
	examp	DD	0
	    b	DD	0
	danik	DD	0
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
	  c11	DD	0
.CODE
	; // ----------- codefunctions declaration -----------


cea2020:


	; // this is examp exsseption!
	push	c1
	push	c2
	pop	eax
	pop	ebx
	add	eax,	ebx
	push	eax
	pop	examp


	; // this is b exsseption!
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


	; // this is danik exsseption!
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

	push	c11	; // this is return of function: main
	call ExitProcess
start:
	jmp cea2020
