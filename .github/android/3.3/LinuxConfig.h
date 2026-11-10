/*
 * Copyright LWJGL. All rights reserved.
 * License terms: https://www.lwjgl.org/license
 */

#include <stddef.h>
#include <inttypes.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

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

#if defined(__ANDROID__) && __ANDROID_API__ < 33

static inline ssize_t lwjgl_pwritev2(int fd, const struct iovec *iov, int iovcnt,
                                     off_t offset, int flags) {
        uint64_t off = (uint64_t)(int64_t)offset;
        return (ssize_t)syscall(SYS_pwritev2, fd, iov, iovcnt,
                                (unsigned long)off, (unsigned long)(off >> 32), flags);
    }

static inline ssize_t lwjgl_preadv2(int fd, const struct iovec *iov, int iovcnt,
                                    off_t offset, int flags) {
        uint64_t off = (uint64_t)(int64_t)offset;
        return (ssize_t)syscall(SYS_preadv2, fd, iov, iovcnt,
                                (unsigned long)off, (unsigned long)(off >> 32), flags);
    }

#define pwritev2(fd, iov, iovcnt, offset, flags) lwjgl_pwritev2((fd), (iov), (iovcnt), (offset), (flags))
#define preadv2(fd, iov, iovcnt, offset, flags)  lwjgl_preadv2((fd), (iov), (iovcnt), (offset), (flags))

#endif
