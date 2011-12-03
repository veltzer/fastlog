#include <fastlog.h> // our own header
#include <pthread.h> // for pthread_mutex_lock(3), pthread_mutex_unlock(3)
#include <stdio.h> // for vsnprintf(3)
#include <stdarg.h> // for va_start(3), va_end(3), va_list
#include <stdlib.h> // for malloc(3)

static const int MAX_MSG=1024;
static pthread_mutex_t fastmutex;
static pthread_spinlock_t fastspin;
static void* buffer=NULL;

void fastlog_config_init(fastlog_config* conf) {
	conf->thread_set=false;
	conf->priority_set=false;
	conf->buffer_size_set=false;
	conf->sleep_time_set=false;
}

void fastlog_init(const fastlog_config* conf) {
	int res;
	res=pthread_mutex_init(&fastmutex,NULL);
	if(res) {
		perror("pthread_mutex_init");
		exit(1);
	}
	res=pthread_spin_init(&fastspin,0);
	if(res) {
		perror("pthread_spin_init");
		exit(1);
	}
	buffer=malloc(2^16);
	if(buffer==NULL) {
		perror("malloc");
		exit(1);
	}
}

void fastlog_close(void) {
	// no return value for error code from this one...
	free(buffer);
	int res;
	res=pthread_spin_destroy(&fastspin);
	if(res) {
		perror("pthread_spin_destroy");
		exit(1);
	}
	res=pthread_mutex_destroy(&fastmutex);
	if(res) {
		perror("pthread_mutex_destroy");
		exit(1);
	}
}

void fastlog_log(const char* fmt,...) {
}

void fastlog_empty(const char* fmt,...) {
}

void fastlog_copy(const char* fmt,...) {
	const unsigned int buffer_size=MAX_MSG;
	char buffer[buffer_size];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, buffer_size, fmt, args);
	va_end(args);
}

void fastlog_mutex(const char* fmt,...) {
	pthread_mutex_lock(&fastmutex);
	va_list ap;
	va_start(ap,fmt);
	fastlog_log(fmt,ap);
	pthread_mutex_unlock(&fastmutex);
}

void fastlog_spin(const char* fmt,...) {
	pthread_spin_lock(&fastspin);
	va_list ap;
	va_start(ap,fmt);
	fastlog_log(fmt,ap);
	pthread_spin_unlock(&fastspin);
}
