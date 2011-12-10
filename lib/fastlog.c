#include <us_helper.h>
#include <fastlog.h> // our own header
#include <pthread.h> // for pthread_create(3), pthread_join(3)
//#include <stdio.h> // for vsnprintf(3), snprintf(3), fputs(3)
#include <stdarg.h> // for va_start(3), va_end(3), va_list
#include <stdlib.h> // for malloc(3), free(3), exit(3)
#include <unistd.h> // for usleep(3)
#include <syslog.h> // for syslog(3)
#include <sys/mman.h> // for mlock(2)

#define DO_WRITE
//#define DO_INIT
//#define DO_SYMSLOW

static char* buffer=NULL;
static char* head;
static char* tail;
static int buflen;
static volatile bool stop;
static pthread_t worker;
static fastlog_config* myconf;
#ifdef DO_INIT
static bool init=false;
#endif // DO_INIT
#ifdef DO_SYMSLOW
static int final;
#endif // DO_SYMSLOW
static FILE* f;

static void* fastlog_worker(void* vconf) {
	//sleep(100);
	while(stop==false) {
		#ifdef DO_SYMSLOW
		int i;
		for(i=0;i<10000;i++) {
			final+=i*i;
		}
		#endif // DO_SYMSLOW
		//usleep(myconf->sleep_time);
		printf("in there\n");
		while(head!=tail) {
			printf("in here\n");
			int res=fputs(tail,f);
			if(res==EOF) {
				perror("fputs");
				exit(1);
			}
			tail+=myconf->buffer_max_msg;
			if(tail==buffer+buflen) {
				tail=buffer;
			}
		}
	}
	while(head!=tail) {
		printf("in here\n");
		int res=fputs(tail,f);
		if(res==EOF) {
			perror("fputs");
			exit(1);
		}
		tail+=myconf->buffer_max_msg;
		if(tail==buffer+buflen) {
			tail=buffer;
		}
	}
	return NULL;
}

void fastlog_config_init(fastlog_config* conf) {
	conf->thread_set=false;
	conf->rt_set=false;
	conf->priority_set=false;
	conf->buffer_msg_num_set=false;
	conf->buffer_max_msg_set=false;
	conf->sleep_time_set=false;
	conf->file_set=false;
	conf->destroy_me=false;
}

void fastlog_init(fastlog_config* conf) {
	if(conf==NULL) {
		conf=(fastlog_config*)malloc(sizeof(fastlog_config));
		fastlog_config_init(conf);
		conf->destroy_me=true;
	}
	if(!conf->thread_set) {
		conf->thread=false;
	}
	if(!conf->rt_set) {
		conf->rt=false;
	}
	if(!conf->priority_set) {
		conf->priority=40;
	}
	if(!conf->buffer_msg_num_set) {
		conf->buffer_msg_num=1000;
	}
	if(!conf->buffer_max_msg_set) {
		conf->buffer_max_msg=1024;
	}
	if(!conf->sleep_time_set) {
		conf->sleep_time=5;
	}
	if(!conf->file_set) {
		snprintf(conf->file,PATH_MAX,"/tmp/foobar");
	}
	myconf=conf;
	buflen=myconf->buffer_msg_num*myconf->buffer_max_msg;
	buffer=(char*)malloc(buflen);
	if(buffer==NULL) {
		perror("malloc");
		goto error;
	}
	int res=mlock(buffer,buflen);
	if(res==-1) {
		perror("mlock");
		goto undo_malloc;
	}
	head=buffer;
	tail=buffer;
	if(!myconf->thread) {
		return;
	}
	stop=false;
	// open the output file
	f=fopen(myconf->file,"w+");
	if(f==NULL) {
		perror("fopen");
		goto undo_malloc;
	}
	// start the background thread
	pthread_attr_t myattr;
	res=pthread_attr_init(&myattr);
	if(res) {
		perror("pthread_attr_init");
		goto undo_open;
	}
	pthread_attr_setinheritsched(&myattr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&myattr, SCHED_FIFO);
	struct sched_param myparam;
	myparam.sched_priority=myconf->priority;
	pthread_attr_setschedparam(&myattr, &myparam);
	res=pthread_create(&worker,&myattr,fastlog_worker,myconf);
	if(res) {
		perror("pthread_create");
		goto undo_open;
	}
	#ifdef DO_INIT
	// from now on we can be used...
	init=true;
	#endif // DO_INIT
	return;

	undo_open:
		res=fclose(f);
		if(res) {
			perror("fclose");
		}
	undo_malloc:
		// no error value from free
		free(buffer);
	error:
		exit(1);
}

void fastlog_close(void) {
	#ifdef DO_INIT
	init=false;
	#endif // DO_INIT
	if(myconf->thread) {
		stop=true;
		int ret=pthread_join(worker,NULL);
		if(ret) {
			perror("pthread_join");
			exit(1);
		}
		ret=fclose(f);
		if(ret) {
			perror("fclose");
			exit(1);
		}
	}
	// no return value for error code from this one...
	free(buffer);
	if(myconf->destroy_me) {
		free(myconf);
	}
}

void fastlog_log(const char* fmt,...) {
	#ifdef DO_INIT
	if(!init) {
		printf("called before init or after close\n");
		exit(1);
	}
	#endif // DO_INIT
	#ifdef DO_WRITE
	// atomically get my own position from head
	// (increase the head and get the old value)
	// apparently there is no need for a loop around this
	// There is no race here with the logging
	// thread since it always grabs the tail.
	// The race here is always with other loggers.
	/*
	char* cur=__sync_fetch_and_add(&head,myconf->buffer_max_msg);
	if(head>=myconf->buffer_len) {
		__sync_bool_compare_and_swap(&head,cur+1,cur+1-myconf->buffer_msg_num);
	}
	cur%=myconf->buffer_msg_num;
	char* pos=buffer+cur*myconf->buffer_max_msg;
	*/
	char* pos=head;
	head+=myconf->buffer_max_msg;
	if(head==buffer+buflen) {
		head=buffer;
	}
	// write to my position
	__builtin_va_list args;
	__builtin_va_start(args, fmt);
	__builtin_vsnprintf(pos, myconf->buffer_max_msg, fmt, args);
	__builtin_va_end(args);
	#endif // DO_WRITE
}
