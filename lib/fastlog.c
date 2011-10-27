#include <fastlog.h> // our own header
#include <pthread.h> // for pthread_mutex_lock(3), pthread_mutex_unlock(3)
#include <stdio.h> // for vsnprintf(3)
#include <stdarg.h> // for va_start(3), va_end(3), va_list

void fastlog_init(void) {
}


void fastlog_close(void) {
}

void fastlog_log(const char* fmt,...) {
}

void fastlog_empty(const char* fmt,...) {
}

void fastlog_copy(const char* fmt,...) {
	const unsigned int buffer_size=1024;
	char buffer[buffer_size];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, buffer_size, fmt, args);
	va_end(args);
}

// this is the mutex we will use to protect the shared memory,
// maybe we should use an atomic instead ?
pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;

void fastlog_mutex(const char* fmt,...) {
	pthread_mutex_lock(&fastmutex);
	const unsigned int buffer_size=1024;
	char buffer[buffer_size];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, buffer_size, fmt, args);
	va_end(args);
	pthread_mutex_unlock(&fastmutex);
}
