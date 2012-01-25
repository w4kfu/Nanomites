	.file	"test.bc"
	.text
	.globl	other_func
	.align	16, 0x90
	.type	other_func,@function
other_func:                             # @other_func
# BB#0:
	subl	$12, %esp
	movl	$0, 8(%esp)
	movl	$0, (%esp)
	calll	exit
.Ltmp0:
	.size	other_func, .Ltmp0-other_func

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
# BB#0:
	subl	$28, %esp
	movl	$0, 24(%esp)
	movl	32(%esp), %eax
	movl	%eax, 20(%esp)
	movl	36(%esp), %eax
	movl	%eax, 16(%esp)
	cmpl	$42, 20(%esp)
	jne	.LBB1_2
# BB#1:
	movl	$.L.str, (%esp)
	jmp	.LBB1_5
.LBB1_2:
	cmpl	$29, 20(%esp)
	jg	.LBB1_4
# BB#3:
	movl	$.L.str1, (%esp)
	jmp	.LBB1_5
.LBB1_4:
	movl	$.L.str2, (%esp)
.LBB1_5:
	calll	printf
	calll	other_func
	#APP
	int3
	#NO_APP
	xorl	%eax, %eax
	addl	$28, %esp
	ret
.Ltmp1:
	.size	main, .Ltmp1-main

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	 "A equal 42"
	.size	.L.str, 11

	.type	.L.str1,@object         # @.str1
.L.str1:
	.asciz	 "A is below 30"
	.size	.L.str1, 14

	.type	.L.str2,@object         # @.str2
.L.str2:
	.asciz	 "A not equal 42"
	.size	.L.str2, 15


	.section	".note.GNU-stack","",@progbits
