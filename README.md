This library implements a portable 'Defer' statement in C89+, allowing
statements to run at function return time, generally to do cleanup.  It
is similar to 'defer' in Go and to 'scope(exit)' in D, but note that it
does not care about lexical scopes.  Cleanup blocks run in reverse order
of the run-time order of `Defer()` invocations.  Integrating `defer.h`
into your codebase is simple; see demo.c for example usage.

Usage:

```
/* Any return type will do, including void */
int
function_name(formals)
{
        Deferral;
        /* Local variable definitions go here */

        BeginDeferral;

        /*
         * Code goes here.  Use the Defer() macro to defer cleanup.
         *
         * One or more Return statement must appear.
         *
         * Do not use the Defer() or Return macros inside switch
         * statements! 
         */
        ...;
        Return 0; /* If function returns void, leave the value out */

        EndDeferral;
        /* NOTREACHED */
}
```

Notes:

 - At the moment, a maximum of 32 deferred statements per function is
   supported.  To change this number, `#define DEFER_MAX_DEFERRED_STATEMENTS`
   to a different value before including defer.h (or via a compiler
   flag).

 - Do not `Return` from inside switch statements in your code.

 - You can change the `Return` macro's name to `return` if you like, but
   then all your functions in a `defer.h`-using file must use the Defer
   macros mentioned above.
