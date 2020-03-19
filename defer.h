#ifndef DEFER_H
#define DEFER_H

#ifndef DEFER_MAX_DEFERRED_STATEMENTS
# define DEFER_MAX_DEFERRED_STATEMENTS 32
#endif

/*
 * Usage:
 *
 *      type
 *      function_name(formals)
 *      {
 *              Deferral;
 *              <locals here>;
 *
 *              BeginDeferral;
 *              <code here>
 *              <at least one Return statement must appear>
 *              EndDeferral;
 *              <NO CODE HERE>
 *      }
 */

#define Deferral \
unsigned char _num_deferrals = 0; \
long _next_block = 0; \
long _defer_return_loc = -1; \
long _deferrals[DEFER_MAX_DEFERRED_STATEMENTS] = {0}
#define BeginDeferral _next_cleanup: switch (_next_block) { \
        default: if (_defer_return_loc > 0) { _next_block = _defer_return_loc; goto _next_cleanup; }; break; \
        case 0: do { 0
#define EndDeferral } while (0); abort(); }

#define Defer(block) _Defer(block, __LINE__)
#define Return _Return(__LINE__)

#define _Defer(block, n) \
        if (_num_deferrals >= DEFER_MAX_DEFERRED_STATEMENTS) abort(); \
	_deferrals[_num_deferrals++] = n; \
	if (0) { \
                case n: \
		block; \
		if (_num_deferrals) { \
			_next_block = _deferrals[--_num_deferrals]; \
                        goto _next_cleanup; \
		} else { \
                        _next_block = -1; \
                        goto _next_cleanup; \
		} \
	}

#define _Return(n) \
        case n: if (_num_deferrals) { \
                _defer_return_loc = n; \
                _next_block = _deferrals[--_num_deferrals]; \
                goto _next_cleanup; \
	} \
        return

#endif /*DEFER_H*/
