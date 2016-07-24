#ifndef __fastlog_h
#define __fastlog_h

#include <stdbool.h> // for bool
#include <limits.h> // for PATH_MAX
#include <pthread.h> // for pthread_t
#include <stdio.h> // for FILE

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Copyright Mark Veltzer <mark.veltzer@gmail.com>, 2011
 */

/*
 * Declaration of the fastlog configuration struct without defining it.
 */
typedef void* fastlog_config_t;

/*
 * This will initialize the config so that all options have default values.
 */
int fastlog_config_init(fastlog_config_t*);
int fastlog_config_destroy(fastlog_config_t*);

/*
 * Setters and getters
 */
int fastlog_config_set_mlock(fastlog_config_t*, const bool mlock);
int fastlog_config_get_mlock(const fastlog_config_t*, bool* mlock);

int fastlog_config_set_msgnum(fastlog_config_t*, const int msgnum);
int fastlog_config_get_msgnum(const fastlog_config_t*, int* msgnum);

int fastlog_config_set_msgsize(fastlog_config_t*, const int msgsize);
int fastlog_config_get_msgsize(const fastlog_config_t*, int* msgsize);

/*
 * Initialize the fastlog system. This will spawn a lower priority thread to do the
 * logging for you. It is an expensive call and should be used once at the begining
 * of the application. Failure to call this one will lead to weird behaviour
 * from fastlog_log.
 */
int fastlog_init(const fastlog_config_t*);

/*
 * Close the fastlog system. This will shut down the thread that does the logging.
 * This is a blocking syscall and may delay you until the thread that does the logging
 * shuts down. Do not call this method at the critical section of your real time
 * application.
 */
int fastlog_close();

/*
 * The real logging method. This method can be used even in time critical threads
 * as it never goes to the kernel and works fast. This is the real workhorse
 * of the fastlog system.
 *
 * There is a reason why all methods except this one return a value: we don't want
 * to force the user to check the return value of this function since the user
 * will be calling this function many many times. This is not so for all the other
 * function here.
 *
 * Is there a chance that this function will fail? Not really. It does all of it's
 * work in RAM with atomic operations.
 */
void fastlog_log(const char* fmt,...) __attribute__((format(printf, 1, 2)));

/*
 * Methods from here are still not implemented
 */

/*
 * This method is a little slow (it requires a syscall) and you should avoid it in your
 * time critical threads unless you have time to spare (at the end of a cycle
 * or what not). It asks the fastlogger to write whatever it has on it's plate by sending
 * a signal. It DOES NOT wait for the logger to write, it just sends the signal.
 */
int fastlog_sendwake();

/*
 * This method is slow. It syncs with the logger (means waits until the logger has
 * cought up). Don't use it in your real time sections unless you really want
 * the logging to catch on and have no problem with the latency.
 */
int fastlog_sync();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __fastlog_h
