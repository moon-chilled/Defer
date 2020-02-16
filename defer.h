#ifndef DEFER_H
#define DEFER_H

#include <setjmp.h>

#ifdef __GNUC__

#define Deferral \
unsigned _num_deferrals = 0; \
void *_defer_return_loc, *_deferrals[24]; /* TODO: make this number configurable? */ \

#define Defer(block) _Defer(block, __COUNTER__)

#define _defer_tokpaste(a, b) a ## b

#define _Defer(block, n) do { \
	_deferrals[_num_deferrals++] = && _defer_tokpaste(_defer_ini, n); \
	goto _defer_tokpaste(_defer_fini_, n); \
	_defer_tokpaste(_defer_ini, n): \
	block; \
	if (_num_deferrals) { \
		goto *_deferrals[--_num_deferrals]; \
	} else { \
		goto *_defer_return_loc; \
	} \
	_defer_tokpaste(_defer_fini_, n): ;\
} while (0)

#define Return(...) _Return(__COUNTER__, __VA_ARGS__)

#define _Return(n, ...) do { \
	if (_num_deferrals) { \
		_defer_return_loc = && _defer_fini_ ## n; \
		goto *_deferrals[--_num_deferrals]; \
	} \
\
	_defer_fini_ ## n: \
	return __VA_ARGS__; \
} while (0)

/* for compatibility */
#define ReturnN Return()

#else

#define Deferral \
volatile unsigned _num_deferrals = 0; \
jmp_buf _defer_return_loc, _deferrals[24]; /* TODO: make this number configurable? */

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
	if (setjmp(_defer_return_loc)) { \
		return __VA_ARGS__; \
	} \
	if (_num_deferrals) { \
		longjmp(_deferrals[--_num_deferrals], 1); \
	} else { \
		longjmp(_defer_return_loc, 1); /* should this just be 'return val'? */\
	} \
} while (0)

/* for compatibility */
# define ReturnN Return()

#else
# define Return(val) do { \
	if (setjmp(_defer_return_loc)) { \
		return val; \
	} \
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

#endif /* __GNUC__ */

#endif /*DEFER_H*/
