#include <stdio.h>

int main(int argc, char *argv[])
{
	 (void)argc; (void)argv;


	// a) 0...01
	unsigned int mask = 1<<0;
	printf("%d\n", mask );

	// b) 0...10
	mask = 1<<1;
	printf("%d\n", mask );
	
	// c) 0...10101
	mask= 1 << 4 | 1<<2 | 1<<1;
	printf("%d\n", mask);


	return 0;
}

