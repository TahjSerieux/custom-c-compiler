	.file	"return_2.c" ;indicates the original source filename
	.text ;marks the beginning of the code section
	.globl	main ;makes the main symbol globally visible (so it can be called from outside)
	.type	main, @function ;declares that main is a function
main:
	movl	$2, %eax ;Moves the literal value 2 into the %eax register.
					 ;The %eax register is used by convention to hold a function's return
					 ;value on x86-64.
	ret   			 ;Returns from the function back to the caller.
	.size	main, .-main; Has the syntax (.size symbol_name, expression)
						;The size of symbol_name is equal to expression
						;Here the size of main is equal to .-main
						; (.) denotes the current address, so here the address after the ret command
	.ident	"GCC: (GNU) 15.2.1 20250813"
	.section	.note.GNU-stack,"",@progbits
