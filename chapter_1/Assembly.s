	.global main
main:
	movl $2, %eax
	ret

	.global chonk
chonk:
	movl $3, %eax
	ret

	.global cat
cat:
	movl $4, %eax
	ret

.section .note.GNU-stack,"",@progbits
