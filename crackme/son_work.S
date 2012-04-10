.global son_work

/* JZ and JE */
.macro nano_jz to
	.byte 0xCC
	.byte 0x1
	.long \to - .
.endm

/* JNZ and JNE */
.macro nano_jnz to
	.byte 0xCC
	.byte 0x2
	.long \to - .
.endm

/* JB and JC */
.macro nano_jb to
	.byte 0xCC
	.byte 0x3
	.long \to - .
.endm

/* JNB and JNC */
.macro nano_jnb to
	.byte 0xCC
	.byte 0x4
	.long \to - .
.endm

/* JMP */
.macro nano_jmp to
	.byte 0xCC
	.byte 0x5
	.long \to - .
.endm

lol:
	xor %eax, %eax
	ret

son_work:

	nano_jz lol
	ret
