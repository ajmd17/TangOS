#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
 
__attribute__((__noreturn__)) void abort(void) {
	printf("Kernel panic: abort() was called\n");
	while (true) {
	}
	__builtin_unreachable();
}
