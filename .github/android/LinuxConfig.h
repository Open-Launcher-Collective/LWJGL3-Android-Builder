/*
 * Copyright LWJGL. All rights reserved.
 * License terms: https://www.lwjgl.org/license
 */

#include <stddef.h>
#include <inttypes.h>

#define DISABLE_WARNINGS() \
    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
    _Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"")
    _Pragma("GCC diagnostic ignored \"-Wsign-compare\"")

#define ENABLE_WARNINGS() \
    _Pragma("GCC diagnostic pop")

// JNIEXPORT_CRITICAL & CRITICAL are used as a workaround for JDK-8167409 on applicable functions.
#define JNIEXPORT_CRITICAL static
#define CRITICAL(function) _JavaCritical_##function

#ifdef __ANDROID__
#define pwritev2(fd, iov, iovcnt, offset, flags)                         \
    ({                                                                   \
        uint64_t _pw2_off = (uint64_t)(offset);                          \
        (ssize_t)syscall(SYS_pwritev2, (int)(fd), (iov), (int)(iovcnt),  \
                         (unsigned long)_pw2_off,                        \
                         (unsigned long)(_pw2_off >> 32),                \
                         (int)(flags));                                  \
    })

#define preadv2(fd, iov, iovcnt, offset, flags)                          \
    ({                                                                   \
        uint64_t _pr2_off = (uint64_t)(offset);                          \
        (ssize_t)syscall(SYS_preadv2, (int)(fd), (iov), (int)(iovcnt),   \
                         (unsigned long)_pr2_off,                        \
                         (unsigned long)(_pr2_off >> 32),                \
                         (int)(flags));                                  \
    })
#endif
