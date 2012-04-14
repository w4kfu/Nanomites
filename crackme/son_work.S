.section .bss

key: .lcomm buffer, 16

.section .data

file:   .byte   0xE2
    .byte   0xEC
    .byte   0xF0
    .byte   0xA7
    .byte   0xEF
    .byte   0xE0
    .byte   0xE5
    .byte   0xEC
    .byte   0

wrong_msg:  .string "You are not authenticated!\n"

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

/* JS */
.macro nano_js to
    .byte 0xCC
    .byte 0x6
    .long \to - .
.endm

syscall:
    int         $0x80
    ret

/* Decrypt str */
decrypt:
    mov         4(%esp), %eax
deb_decrypt:
    cmpb        $0, (%eax)
    nano_jz     end_decrypt
    xorb        $0x89, (%eax)
    inc         %eax
    nano_jmp    deb_decrypt
end_decrypt:
    ret         $4

/* Open key file */
open:
    xor         %eax, %eax
    mov         $file, %ebx
    mov         $5, %eax /* sys_open */
    xor         %ecx, %ecx
    push        $test_open
    nano_jmp    syscall

test_open:
    test        %eax, %eax
    nano_js     wrong
    mov         %eax, %esi
    nano_jmp    exit

son_work:

    push        $file
    push        $son_work_1
    nano_jmp    decrypt

son_work_1:
    nano_jmp    open

wrong:
    mov         $4, %eax /* sys_write */
    mov         $1, %ebx
    mov         $27, %edx
    mov         $wrong_msg, %ecx
    push        $exit
    nano_jmp    syscall

exit:
    xor         %eax, %eax
    movb        $1, %al /* sys_exit */
    xor         %ebx, %ebx
    nano_jmp    syscall

