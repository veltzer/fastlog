#include <us_helper.h>
#include <fastlog.h> // our own header
#include <pthread.h> // for pthread_create(3), pthread_join(3)
#include <stdio.h> // for vsnprintf(3), snprintf(3), fputs(3)
#include <stdarg.h> // for va_start(3), va_end(3), va_list
#include <stdlib.h> // for malloc(3), free(3), exit(3)
#include <unistd.h> // for usleep(3)
#include <syslog.h> // for syslog(3)
#include <sys/mman.h> // for mlock(2)

//#define DO_WRITE

typedef struct _fastlog_config {
	// this is our own pointer for fastlog internal use
	// this is to keep the user interface constant.
	void* internal;
	// room for more data with aby compatibility
	// buffer related
	// these options are to be configured by the user

	/*
	Should we lock the buffer we use for fastlog?
	You must have permission to lock the size of the buffer if you
	configure this to true.
	default: false
	*/
	bool mlock;
	int buffer_msg_num;
	int buffer_max_msg;
	int buflen;
	char* buffer;
	char* head;
	char* tail;

} fastlog_config;

typedef struct _fastlog_thread_config {
	// these options are to be configured by the user
	bool thread;
	bool rt;
	int priority;
	int sleep_time;

	// the pthread worker
	pthread_t worker;
	// stop the background thread
	volatile bool stop;
	// the file the background thread writes to
	FILE* file;
} fastlog_thread_config;

static void* fastlog_worker(void* vconf) {
	/*
	fastlog_thread_config* conf=(fastlog_thread_config*)vconf;
	while(conf->stop==false) {
		while(conf->head!=conf->tail) {
			int res=fputs(conf->tail,conf->file);
			if(res==EOF) {
				perror("fputs");
				exit(1);
			}
			conf->tail+=conf->buffer_max_msg;
			if(conf->tail==conf->buffer+conf->buflen) {
				conf->tail=conf->buffer;
			}
		}
	}
	while(conf->head!=conf->tail) {
		int res=fputs(conf->tail,conf->file);
		if(res==EOF) {
			perror("fputs");
			exit(1);
		}
		conf->tail+=conf->buffer_max_msg;
		if(conf->tail==conf->buffer+conf->buflen) {
			conf->tail=conf->buffer;
		}
	}
	*/
	return NULL;
}

static inline int _fastlog_config_init(fastlog_config* fc) {
	fc->mlock=false;
	fc->buffer_msg_num=1024;
	fc->buffer_max_msg=1024;
	fc->buffer=NULL;
	return 0;
}

int fastlog_config_init(fastlog_config_t* conf) {
	fastlog_config* fc=(fastlog_config*)malloc(sizeof(fastlog_config));
	_fastlog_config_init(fc);
	*conf=fc;
	return 0;
}

int fastlog_config_destroy(fastlog_config_t* conf) {
	fastlog_config* fc=*conf;
	free(fc);
	return 0;
}

int fastlog_thread_config_init(fastlog_thread_config* conf) {
	conf->thread=false;
	conf->rt=false;
	conf->priority=55;
	conf->sleep_time=5;
	return 0;
}

int fastlog_init(const fastlog_config_t* conf) {
	fastlog_config* fc;
	if(conf==NULL) {
		fastlog_config config;
		_fastlog_config_init(&config);
		fc=&config;
	} else {
		fc=*conf;
	}
	fc->buflen=fc->buffer_msg_num*fc->buffer_max_msg;
	fc->buffer=(char*)malloc(fc->buflen);
	if(fc->buffer==NULL) {
		perror("malloc");
		goto error;
	}
	if(fc->mlock) {
		int res=mlock(fc->buffer,fc->buflen);
		if(res==-1) {
			perror("mlock");
			goto undo_malloc;
		}
	}
	// set the head and tail
	fc->head=fc->buffer;
	fc->tail=fc->buffer;
	return 0;

	/* error handling */
undo_malloc:
	// no error value from free
	free(fc->buffer);
error:
	return 0;
}

int fastlog_thread_start(fastlog_thread_config* conf) {
	pthread_attr_t myattr;
	int res=pthread_attr_init(&myattr);
	if(res) {
		perror("pthread_attr_init");
		goto error;
	}
	pthread_attr_setinheritsched(&myattr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&myattr, SCHED_FIFO);
	struct sched_param myparam;
	myparam.sched_priority=conf->priority;
	pthread_attr_setschedparam(&myattr, &myparam);
	res=pthread_create(&conf->worker,&myattr,fastlog_worker,conf);
	if(res) {
		perror("pthread_create");
		goto error;
	}
	conf->stop=false;
	return 0;

error:
	return res;
}

int fastlog_close(fastlog_config* conf) {
	// no return value for error code from this one...
	free(conf->buffer);
	return 0;
}

void fastlog_thread_shutdown(fastlog_thread_config* conf) {
	conf->stop=true;
	int ret=pthread_join(conf->worker,NULL);
	if(ret) {
		perror("pthread_join");
		exit(1);
	}
	ret=fclose(conf->file);
	if(ret) {
		perror("fclose");
		exit(1);
	}
}

void fastlog_log(const char* fmt,...) {
	// atomically get my own position from head
	// (increase the head and get the old value)
	// apparently there is no need for a loop around this
	// There is no race here with the logging
	// thread since it always grabs the tail.
	// The race here is always with other loggers.
	/*
	char* pos=__sync_fetch_and_add(&conf->head,conf->buffer_max_msg);
	if(conf->head>=conf->buffer_len) {
		__sync_bool_compare_and_swap(&conf->head,cur+1,cur+1-conf->buffer_msg_num);
	}
	cur%=conf->conf->buffer_msg_num;
	char* pos=buffer+cur*conf->buffer_max_msg;
	*/
	#ifdef DO_WRITE
	char* pos __attribute__((unused))=conf->head;
	conf->head+=conf->buffer_max_msg;
	if(conf->head==conf->buffer+conf->buflen) {
		conf->head=conf->buffer;
	}
	// write to my position
	// simulate the copy (not accurate)
	memcpy(pos,&fmt,8);
	/*
	__builtin_va_list args;
	__builtin_va_start(args, fmt);
	__builtin_vsnprintf(pos, conf->buffer_max_msg, fmt, args);
	__builtin_va_end(args);
	*/
	#endif // DO_WRITE
}
