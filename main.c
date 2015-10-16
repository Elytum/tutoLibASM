int ft_is_A(char c);

#include <stdio.h>

void	do_test(char c)
{
	printf("%c returned: %i\n", c, ft_is_A(c));
}

int main(void)
{
	do_test('a');
	do_test('c');
	do_test('B');
	do_test('A');
	do_test('d');
}