	.file	"mystery.c"
	.text
.globl mystery
	.type	mystery, @function
mystery:
	pushq	%rbp
	movq	%rsp, %rbp
	
	movl	%edi, -20(%rbp)
	movl	$1, -16(%rbp)
	movl	$0, -12(%rbp)
	movl	$0, -8(%rbp)
	cmpl	$2, -20(%rbp)
	jg	.L2
	movl	$1, %eax
	jmp	.L3
.L2:
	movl	$2, -4(%rbp)
	jmp	.L4
.L5:
	movl	-12(%rbp), %eax
	movl	-16(%rbp), %edx
	leal	(%rdx,%rax), %eax
	movl	%eax, -8(%rbp)
	movl	-16(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, -16(%rbp)
	addl	$1, -4(%rbp)
.L4:
	movl	-4(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jle	.L5
	movl	-8(%rbp), %eax
.L3:
	leave
	ret
