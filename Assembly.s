	.global main
main:
	subq $0, %rsp
	movl $2, %eax
	movq %rbp, %rsp
	popq %rbp
	ret

	.global chonk
chonk:
	subq $-12, %rsp
	movl $3, -4(%rbp)
	negl -4(%rbp)
	movl -4(%rbp), %r10d
	movl %r10d, -8(%rbp)
	notl -8(%rbp)
	movl -8(%rbp), %r10d
	movl %r10d, -12(%rbp)
	negl -12(%rbp)
	movl -12(%rbp), %eax
	movq %rbp, %rsp
	popq %rbp
	ret

	.global cat
cat:
	subq $-4, %rsp
	movl $44324, -4(%rbp)
	negl -4(%rbp)
	movl -4(%rbp), %eax
	movq %rbp, %rsp
	popq %rbp
	ret

.section .note.GNU-stack,"",@progbits
