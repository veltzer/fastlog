#ifndef __fastlog_h
#define __fastlog_h

#include<stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Copyright Mark Veltzer <mark.veltzer@gmail.com>, 2011
 */

/*
 * Fastlog configuration structure.
 */

typedef struct _fastlog_config {
	bool thread_set;
	bool thread;
	bool rt_set;
	bool rt;
	bool priority_set;
	int priority;
	bool buffer_size_set;
	int buffer_size;
	bool sleep_time_set;
	int sleep_time;
} fastlog_config;

/*
 * This will initialize the config so that nothing is set
 */
void fastlog_config_init(fastlog_config*);

/*
 * Initialize the fastlog system. This will spawn a lower priority thread to do the
 * logging for you. It is an expensive call and should be used once at the begining
 * of the application. Failure to call this one will lead to weird behaviour
 * from fastlog_log.
 */
void fastlog_init(const fastlog_config*);

/*
 * Close the fastlog system. This will shut down the thread that does the logging.
 * This is a blocking syscall and may delay you until the thread that does the logging
 * shuts down. Do not call this method at the critical section of your real time
 * application.
 */
void fastlog_close(void);

/*
 * The real logging method. This method can be used even in time critical threads
 * as it never goes to the kernel and works fast. This is the real workhorse
 * of the fastlog system.
 */
void fastlog_log(const char* fmt,...) __attribute__((format(printf, 1, 2)));

/*
 * This method is blocking and you should avoid it in your
 * time critical threads unless you have time to spare (at the end of a cycle
 * or what not). It asks the fastlogger to write whatever it has on it's plate.
 */

/*
 * Methods for performance testing
 */
void fastlog_empty(const char* fmt,...) __attribute__((format(printf, 1, 2)));
void fastlog_copy(const char* fmt,...) __attribute__((format(printf, 1, 2)));
void fastlog_spin(const char* fmt,...) __attribute__((format(printf, 1, 2)));
void fastlog_mutex(const char* fmt,...) __attribute__((format(printf, 1, 2)));

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __fastlog_h
