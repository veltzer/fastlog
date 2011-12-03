#ifndef __fastlog_h
#define __fastlog_h

#include <stdbool.h> // for bool
#include <limits.h> // for PATH_MAX

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
	bool buffer_msg_num_set;
	int buffer_msg_num;
	bool buffer_max_msg_set;
	int buffer_max_msg;
	bool sleep_time_set;
	int sleep_time;
	bool file_set;
	char file[PATH_MAX];
	// special destroy me
	bool destroy_me;
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
void fastlog_init(fastlog_config*);

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
 * This method is a little slow (it requires a syscall) and you should avoid it in your
 * time critical threads unless you have time to spare (at the end of a cycle
 * or what not). It asks the fastlogger to write whatever it has on it's plate by sending
 * a signal. It DOES NOT wait for the logger to write, it just sends the signal.
 */
void fastlog_sendwake();

/*
 * This method is slow. It syncs with the logger (means waits until the logger has
 * cought up). Don't use it in your real time sections unless you really want
 * the logging to catch on and have no problem with the latency.
 */
void fastlog_sync();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __fastlog_h
