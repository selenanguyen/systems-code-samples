	.file	"array2.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$1600, %edi
	call	malloc
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	addq	$264, %rax
	movl	$712, (%rax)
	movq	-8(%rbp), %rax
	addq	$280, %rax
	movl	$536, (%rax)
	movq	-8(%rbp), %rax
	addq	$1000, %rax
	movl	$10, (%rax)
	movq	-8(%rbp), %rax
	addq	$1400, %rax
	movl	$300, (%rax)
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-39)"
	.section	.note.GNU-stack,"",@progbits
