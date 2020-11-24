.686
.MODEL FLAT, C
.STACK 4096
ExitProcess PROTO, :DWORD

.DATA
	 name	DD	"0",0
	  age	DD	0
	 info	DD	"0",0
	    a	DD	0
	vnvik	DD	"0",0
	examp	DD	0
.CONST
	   c1	DD	23
	   c2	DD	"4",0
	   c3	DD	1
	   c4	DD	1
	   c5	DD	1
	   c6	DD	1
	   c7	DD	1
	   c8	DD	7
	   c9	DD	3
	  c10	DD	12
	  c11	DD	0
	  c12	DD	3
	  c13	DD	4
	  c14	DD	3
	  c15	DD	7
	  c16	DD	7
	  c17	DD	7
	  c18	DD	7
	  c19	DD	7
	  c20	DD	7
	  c21	DD	7
	  c22	DD	7
	  c23	DD	7
	  c24	DD	7
	  c25	DD	5
	  c26	DD	1
	  c27	DD	2
	  c28	DD	3
	  c29	DD	5
	  c30	DD	6
	  c31	DD	1
	  c32	DD	2
	  c33	DD	3
	  c34	DD	4
	  c35	DD	5
	  c36	DD	4
	  c37	DD	7
	  c38	DD	7
	  c39	DD	7
	  c40	DD	7
	  c41	DD	5
	  c42	DD	69
	  c43	DD	0
.CONST
	; // ----------- EXTRN functions declarations -----------


	; // ----------- codefunctions declaration -----------


cea2020:
	; // this special plase for code
	INVOKE ExitProcess, 0
start:
	jmp cea2020
