#ifndef DEFER_H
#define DEFER_H

#include <setjmp.h>

#define Deferral \
size_t _num_deferrals = 0; \
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

#if defined(__STDC_VERSION__) &&  __STDC_VERSION__ >= 199901L
# define Return(...) do { \
	if (setjmp(_defer_return_loc)) return __VA_ARGS__; \
	if (_num_deferrals) { \
		longjmp(_deferrals[--_num_deferrals], 1); \
	} else { \
		longjmp(_defer_return_loc, 1); /* should this just be 'return val'? */\
	} \
} while (0)
#else
# define Return(val) do { \
	if (setjmp(_defer_return_loc)) return val; \
	if (_num_deferrals) { \
		longjmp(_deferrals[--_num_deferrals], 1); \
	} else { \
		longjmp(_defer_return_loc, 1); /* should this just be 'return val'? */\
	} \
} while (0)

# define ReturnN do { \
	if (setjmp(_defer_return_loc)) return; \
	if (_num_deferrals) { \
		longjmp(_deferrals[--_num_deferrals], 1); \
	} else { \
		return; \
	} \
} while (0)
#endif

#endif /*DEFER_H*/
