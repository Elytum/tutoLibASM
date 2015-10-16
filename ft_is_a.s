section .text
	global _ft_is_A

_ft_is_A:
	cmp sil, 'A'
		je do_ret1
	mov rax, 0
	ret

do_ret1:
	mov rax, 1
	ret