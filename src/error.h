/**
 * Copyright (C) 2005-2008 Christoph Rupp (chris@crupp.de).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 *
 * See file COPYING.GPL2 and COPYING.GPL3 for License information.
 * 
 *
 * this file has error logging routines and a strerror() replacement
 *
 */

#ifndef HAM_ERROR_H__
#define HAM_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif 

#include <ham/types.h>

/*
 * Debug output levels
 */
#define DBG_LVL_DEBUG       0
#define DBG_LVL_NORMAL      1
#define DBG_LVL_FATAL       3

/*
 * function prototypes
 */
extern void dbg_lock(void);
extern void dbg_unlock(void);
extern void dbg_prepare(int level, const char *file, int line, 
        const char *function, const char *expr);
extern void dbg_log(const char *format, ...);
extern void dbg_verify_failed(const char *format, ...);

/*
 * a hook for unittests
 */
extern void (*ham_test_abort)(void);

/*
 * if your compiler does not support __FUNCTION__, you can define it here:
 *    #define __FUNCTION__ 0
 */

/** 
 * in debug mode we write trace()-messages to stderr, and assert() 
 * is enabled.
 *
 * not every preprocessor supports ellipsis as macro-arguments -
 * therefore we have to use brackets, so preprocessors treat multiple
 * arguments like a single argument. and we need to lock the output, 
 * otherwise we are not thread-safe. this is super-ugly.
 */
#ifdef HAM_DEBUG
#   define ham_assert(e, f)  if (!(e)) {                               \
                                dbg_lock();                            \
                                dbg_prepare(DBG_LVL_FATAL, __FILE__,   \
                                        __LINE__, __FUNCTION__, #e);   \
                                dbg_verify_failed f;                   \
                                dbg_unlock();                          \
                             }
#else /* !HAM_DEBUG */
#   define ham_assert(e, f)     
#endif /* HAM_DEBUG */

/**
 * log() and verify() are available in every build
 */
#define ham_trace(f)         do {                                      \
                                dbg_lock();                            \
                                dbg_prepare(DBG_LVL_DEBUG, __FILE__,   \
                                        __LINE__, __FUNCTION__, 0);    \
                                dbg_log f;                             \
                                dbg_unlock();                          \
                             } while (0)
#define ham_log(f)           do {                                      \
                                dbg_lock();                            \
                                dbg_prepare(DBG_LVL_NORMAL, __FILE__,  \
                                        __LINE__, __FUNCTION__, 0);    \
                                dbg_log f;                             \
                                dbg_unlock();                          \
                             } while (0)
#define ham_verify(e, f)     if (!(e)) {                               \
                                dbg_lock();                            \
                                dbg_prepare(DBG_LVL_FATAL, __FILE__,   \
                                        __LINE__, __FUNCTION__, #e);   \
                                dbg_verify_failed f;                   \
                                dbg_unlock();                          \
                             }
#ifdef __cplusplus
} // extern "C"
#endif 

#endif /* HAM_ERROR_H__ */
