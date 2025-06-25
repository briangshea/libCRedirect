
#ifndef CREDIRECT_EXPORT_H
#define CREDIRECT_EXPORT_H

#ifdef CREDIRECT_STATIC_DEFINE
#  define CREDIRECT_EXPORT
#  define CREDIRECT_NO_EXPORT
#else
#  ifndef CREDIRECT_EXPORT
#    ifdef CRedirect_EXPORTS
        /* We are building this library */
#      define CREDIRECT_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CREDIRECT_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef CREDIRECT_NO_EXPORT
#    define CREDIRECT_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef CREDIRECT_DEPRECATED
#  define CREDIRECT_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CREDIRECT_DEPRECATED_EXPORT
#  define CREDIRECT_DEPRECATED_EXPORT CREDIRECT_EXPORT CREDIRECT_DEPRECATED
#endif

#ifndef CREDIRECT_DEPRECATED_NO_EXPORT
#  define CREDIRECT_DEPRECATED_NO_EXPORT CREDIRECT_NO_EXPORT CREDIRECT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CREDIRECT_NO_DEPRECATED
#    define CREDIRECT_NO_DEPRECATED
#  endif
#endif

#endif /* CREDIRECT_EXPORT_H */
