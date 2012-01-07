#include <us_helper.h>
#include <fastlog.h> // our own header
#include <pthread.h> // for pthread_create(3), pthread_join(3)
#include <stdio.h> // for vsnprintf(3), snprintf(3), fputs(3)
#include <stdarg.h> // for va_start(3), va_end(3), va_list
#include <stdlib.h> // for malloc(3), free(3), exit(3)
#include <unistd.h> // for usleep(3)
#include <syslog.h> // for syslog(3)
#include <sys/mman.h> // for mlock(2)

#define DO_WRITE

static void* fastlog_worker(void* vconf) {
	fastlog_config* conf=(fastlog_config*)vconf;
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
	return NULL;
}

void fastlog_config_init(fastlog_config* conf) {
	// thread parameters
	conf->thread=false;
	conf->rt=false;
	conf->priority=55;
	conf->sleep_time=5;
	// buffer parameters
	conf->mlock=false;
	conf->buffer_msg_num=1000;
	conf->buffer_max_msg=1024;
	conf->buffer=NULL;
	// operational
	conf->destroy_me=false;
}

void fastlog_init(fastlog_config* conf) {
	if(conf==NULL) {
		conf=(fastlog_config*)malloc(sizeof(fastlog_config));
		fastlog_config_init(conf);
		conf->destroy_me=true;
	}
	conf->buflen=conf->buffer_msg_num*conf->buffer_max_msg;
	conf->buffer=(char*)malloc(conf->buflen);
	if(conf->buffer==NULL) {
		perror("malloc");
		goto error;
	}
	if(conf->mlock) {
		int res=mlock(conf->buffer,conf->buflen);
		if(res==-1) {
			char errbuf[1024];
			snprintf(errbuf,1024,"mlock of [%d] bytes",conf->buflen);
			perror(errbuf);
			goto undo_malloc;
		}
	}
	// set the head and tail
	conf->head=conf->buffer;
	conf->tail=conf->buffer;
	// do the background thread...
	if(conf->thread) {
		pthread_attr_t myattr;
		int res=pthread_attr_init(&myattr);
		if(res) {
			perror("pthread_attr_init");
			goto undo_open;
		}
		pthread_attr_setinheritsched(&myattr, PTHREAD_EXPLICIT_SCHED);
		pthread_attr_setschedpolicy(&myattr, SCHED_FIFO);
		struct sched_param myparam;
		myparam.sched_priority=conf->priority;
		pthread_attr_setschedparam(&myattr, &myparam);
		res=pthread_create(&conf->worker,&myattr,fastlog_worker,conf);
		if(res) {
			perror("pthread_create");
			goto undo_open;
		}
		conf->stop=false;
	}
	return;

	int res;
	undo_open:
		res=fclose(conf->file);
		if(res) {
			perror("fclose");
		}
	undo_malloc:
		// no error value from free
		free(conf->buffer);
	error:
		exit(1);
}

void fastlog_close(fastlog_config* conf) {
	if(conf->thread) {
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
	// no return value for error code from this one...
	free(conf->buffer);
	if(conf->destroy_me) {
		free(conf);
	}
}

void fastlog_log(fastlog_config* conf,const char* fmt,...) {
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
	/*
	__builtin_va_list args;
	__builtin_va_start(args, fmt);
	__builtin_vsnprintf(pos, conf->buffer_max_msg, fmt, args);
	__builtin_va_end(args);
	*/
	#endif // DO_WRITE
}
