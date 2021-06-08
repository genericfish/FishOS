#pragma once

#define __fsh_libc 1

#ifndef ALWAYS_INLINE
#    define ALWAYS_INLINE inline __attribute__((always_inline))
#endif

#if __cplusplus
#    ifndef __BEGIN_DECLS
#        define __BEGIN_DECLS extern "C" {
#        define __END_DECLS   }
#    endif
#else
#    ifndef __BEGIN_DECLS
#        define __BEGIN_DECLS
#        define __END_DECLS
#    endif
#endif
