.section .data

file:	.byte	0xA6
	.byte	0xEC
	.byte	0xFD
	.byte	0xEA
	.byte	0xA6
	.byte	0xF9
	.byte	0xE8
	.byte	0xFA
	.byte	0xFA
	.byte	0xFE
	.byte	0xED
	.byte	0

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

/* Decrypt str */
decrypt:
	mov		4(%esp), %eax
deb_decrypt:
	cmpb		$0, (%eax)
	nano_jz		end_decrypt
	xorb		$0x89, (%eax)
	inc		%eax
	nano_jmp	deb_decrypt
end_decrypt:
	ret		$4


lol:
	xor	%eax, %eax
	mov 	$file, %ebx
	movb 	$5, %al /* sys_open */
	xor 	%ecx, %ecx
	int 	$0x80

        test %eax, %eax
	js exit
	mov %eax, %esi
	jmp read

read:
	mov %esi, %ebx
	movb $3, %al /* sys_read */
	sub $1, %esp
	lea (%esp), %ecx
	movb $1, %dl
	int $0x80

	xor %ebx, %ebx
	cmp %eax, %ebx
	je exit

	movb $4, %al /* sys_write */
	movb $1, %bl
	movb $1, %dl
	int $0x80
	
	add $1, %esp
	jmp read
	
	
	ret

son_work:

	push	$file
	call	decrypt
	/* nano_jmp lol */
	//call	lol
exit:
	xor %eax, %eax
	movb $1, %al /* sys_exit */
	xor %ebx, %ebx
	int $0x80
