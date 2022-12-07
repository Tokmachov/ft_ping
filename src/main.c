#include <stdio.h>
#include "do_something1.h"
#include "do_something2.h"

int main(int ac, char **av)
{	ac = (int)ac;
	av = (char **) av; 
	printf("hello world from main.\n");
	do_something1();
	do_something2();
	return 0;
}
