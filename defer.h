#ifndef DEFER_H
#define DEFER_H

#include <setjmp.h>

#ifdef __GNUC__

#define Deferral \
unsigned _num_deferrals = 0; \
void *_defer_return_loc = 0, *_deferrals[24] = {0}; /* TODO: make this number configurable? */ \

#define Defer(block) _Defer(block, __COUNTER__)

#define _defer_tokpaste(a, b) a ## b

#define _Defer(block, n) do { \
	_deferrals[_num_deferrals++] = && _defer_tokpaste(_defer_ini, n); \
	if (0) { \
		_defer_tokpaste(_defer_ini, n): \
		block; \
		if (_num_deferrals) { \
			goto *_deferrals[--_num_deferrals]; \
		} else { \
			goto *_defer_return_loc; \
		} \
	} \
} while (0)

#define Return _Return(__COUNTER__)

#define _Return(n) \
	if (_num_deferrals) { \
		_defer_return_loc = && _defer_fini_ ## n; \
		goto *_deferrals[--_num_deferrals]; \
	} \
\
	_defer_fini_ ## n: \
	return

#else

#warning You are using the unsafe longjmp()-based defer implementation.  Expect bugs if you don't know what you're doing.

#define Deferral \
volatile unsigned _num_deferrals = 0; \
jmp_buf _defer_return_loc = {0}, _deferrals[24] = {0}; /* TODO: make this number configurable? */

#define Defer(block) do { \
	if (setjmp(_deferrals[_num_deferrals++])) { \
		block; \
		if (_num_deferrals) { \
			longjmp(_deferrals[--_num_deferrals], 1); \
		} else { \
			longjmp(_defer_return_loc, 1); \
		} \
	} \
} while (0)

#define Return do { \
	if (setjmp(_defer_return_loc)) break; \
\
	if (_num_deferrals) { \
		longjmp(_deferrals[--_num_deferrals], 1); \
	} \
} while (0); return

#endif /* __GNUC__ */

#endif /*DEFER_H*/
