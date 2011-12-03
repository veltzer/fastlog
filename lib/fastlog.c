#include <fastlog.h> // our own header
#include <pthread.h> // for pthread_create(3)
#include <stdio.h> // for vsnprintf(3)
#include <stdarg.h> // for va_start(3), va_end(3), va_list
#include <stdlib.h> // for malloc(3), free(3)

static const int MAX_MSG=1024;
static const int MSG_NUM=100;
static void* buffer=NULL;
static int current_msg;

void fastlog_config_init(fastlog_config* conf) {
	conf->thread_set=false;
	conf->rt_set=false;
	conf->priority_set=false;
	conf->buffer_size_set=false;
	conf->sleep_time_set=false;
}

void fastlog_init(const fastlog_config* conf) {
	buffer=malloc(MAX_MSG*MSG_NUM);
	if(buffer==NULL) {
		perror("malloc");
		exit(1);
	}
	current_msg=0;
}

void fastlog_close(void) {
	// no return value for error code from this one...
	free(buffer);
}

void fastlog_log(const char* fmt,...) {
	// atomically get my own position from current_msg
	// (increase the current_msg and get the old value)
	// this should be done in a loop in case of race with
	// another thread doing the same.
	// There is no race here with the logging
	// thread since it always grabs the tail.
	char* pos=((char*)buffer)+current_msg*MAX_MSG;
	// write to my position
	va_list args;
	va_start(args, fmt);
	vsnprintf(pos, MAX_MSG, fmt, args);
	va_end(args);
	// advance the current message
	current_msg++;
	current_msg%=MSG_NUM;
}
