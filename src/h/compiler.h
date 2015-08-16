
#ifdef __GNUC__
#define __noreturn			__attribute__((noreturn))
#else
#define __noreturn
#endif

#define __always_inline         inline __attribute__((always_inline))

// Force always-inline if gcc is too old.
#if (__GNUC__ < 4)
#define inline         inline          __attribute__((always_inline))
#define __inline__     __inline__      __attribute__((always_inline))
#define __inline       __inline        __attribute__((always_inline))
#endif
