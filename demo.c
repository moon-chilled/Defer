#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defer.h"

void test1(void) {Deferral;
	char *x = malloc(0x100);

        BeginDeferral;
	Defer(free(x));

	/* legal because deconstruction happens in reverse order */
	Defer({
		printf("x is %s\n", x);
		x[1]++;
		printf("now %s\n", x);
	});

	strcpy(x, "Hello world");

	Return;
        EndDeferral;
}

int test2(void) {Deferral;
        BeginDeferral;
	puts("1");

	/* caveat: unlike in other programming languages, the return expression
	 * is evaluated after deferred statements.  Unfortunately, there is no
	 * good way around this.
	 */
	Defer(puts("2"));
	Return puts("3");
        EndDeferral;
}

int main(void) {
	test1();
	return test2();
}
